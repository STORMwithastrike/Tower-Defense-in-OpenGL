#include <glad/glad.h>
#include "../headers/ActionManager.h"
#include "../headers/DefensePlayer.h"
#include "../headers/Economy.h"
#include "../headers/Environment.h"
#include "../headers/Grid.h"
#include "../headers/HealthUI.h"
#include "../headers/HomeBase.h"
#include "../headers/InputManager.h"
#include "../headers/LaneSelector.h"
#include "../headers/MeleePlayer.h"
#include "../headers/Player.h"
#include "../headers/PlayerCardUI.h"
#include "../headers/Projectile.h"
#include "../headers/RangedPlayer.h"
#include "../headers/WaveManager.h"
#include "../headers/Window.h"

#include "render/Camera.h"
#include "render/Renderer.h"
#include "render/Shader.h"
#include "render/VertexArray.h"

#include "core/CutsceneManager.h"
#include "core/GameStateMachine.h"

#include "game/Collectible.h"
#include "game/CollisionSystem.h"
#include "game/CombatSystem.h"
#include "game/EasyEnemy.h"
#include "game/EnemyProjectile.h"
#include "game/HardEnemy.h"
#include "game/MediumEnemy.h"

#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <vector>

int main() {
    const float screenWidth = 800.0f;
    const float screenHeight = 600.0f;

    Window gameWindow(static_cast<int>(screenWidth),
        static_cast<int>(screenHeight));
    if (!gameWindow.initialize("Castle Defense - Protect the Princess")) {
        std::cerr << "Failed to initialize Game Window!" << std::endl;
        return -1;
    }

    GLFWwindow* rawWindow = gameWindow.getGLFWWindow();
    glfwSetKeyCallback(rawWindow, InputManager::keyCallback);
    glfwSetMouseButtonCallback(rawWindow, InputManager::mouseButtonCallback);
    glfwSetCursorPosCallback(rawWindow, InputManager::cursorPosCallback);

    // 1. Shaders, Camera & Renderer
    Shader mainShader(ASSETS_DIR "shaders/main.vert",
        ASSETS_DIR "shaders/main.frag");
    Camera camera(screenWidth, screenHeight);
    Renderer renderer(&mainShader, screenWidth, screenHeight);
    renderer.SetCamera(&camera);
    Renderer& myRenderer = renderer;

    // 2. Playfield Grid & Environment
    Grid gameGrid;
    gameGrid.init();
    VertexArray gridVAO(gameGrid.getVertices(), gameGrid.getIndices());

    Environment environment;
    environment.init();
    VertexArray envVAO(environment.getVertices(), environment.getIndices());

    // 3. Home Base (Castle Citadel on the left, starts with 15 HP)
    HomeBase homeBase(0.0f, 60.0f, 98.0f, 480.0f, 15);
    LaneSelector laneSelector;

    // 4. State Machine & Cutscene
    GameStateMachine stateMachine(screenWidth, screenHeight);
    CutsceneManager cutscene(screenWidth, screenHeight);

    // 5. Wave & Combat Systems
    WaveManager waveManager;
    CollisionSystem collisionSystem;
    CombatSystem combatSystem;

    // 6. Players (Friendly Blue Defenders)
    MeleePlayer meleePlayer(0);
    meleePlayer.init(120.0f, gameGrid.getLaneCenterY(0));
    VertexArray meleeVAO(meleePlayer.getVertices(), meleePlayer.getIndices());

    RangedPlayer rangedPlayer(1);
    rangedPlayer.init(120.0f, gameGrid.getLaneCenterY(1));
    VertexArray rangedVAO(rangedPlayer.getVertices(), rangedPlayer.getIndices());

    DefensePlayer defensePlayer(2);
    defensePlayer.init(120.0f, gameGrid.getLaneCenterY(2));
    VertexArray defenseVAO(defensePlayer.getVertices(),
        defensePlayer.getIndices());

    std::vector<Player*> activePlayers = { &meleePlayer, &rangedPlayer,
                                           &defensePlayer };

    // Player Projectiles
    std::vector<Projectile> playerArrows;
    Projectile arrowProto;
    arrowProto.init(0.0f, 0.0f);
    VertexArray projectileVAO(arrowProto.getVertices(), arrowProto.getIndices());

    // Quad VAO for HealthUI
    std::vector<float> quadVertices = {
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f
    };
    std::vector<unsigned int> quadIndices = {
        0, 1, 2,
        1, 3, 2
    };
    VertexArray quadVAO(quadVertices, quadIndices);

    // Enemy Entities, Hostile Projectiles & Collectibles
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<EnemyProjectile> enemyProjectiles;
    std::vector<Collectible> collectibles;

    // 7. Economy & Player Cards UI
    Economy economy(30);

    std::vector<PlayerCardUI> unitCards;
    // Positioned neatly inside the top HUD bar between WAVE and HINTS
    unitCards.emplace_back(CardUnitType::Melee, 10, glm::vec2(245.0f, 6.0f),
        glm::vec2(80.0f, 38.0f));
    unitCards.emplace_back(CardUnitType::Ranged, 15, glm::vec2(335.0f, 6.0f),
        glm::vec2(80.0f, 38.0f));
    unitCards.emplace_back(CardUnitType::Defense, 20, glm::vec2(425.0f, 6.0f),
        glm::vec2(80.0f, 38.0f));

    // Reset helper
    auto restartGame = [&]() {
        economy.reset();
        for (auto& card : unitCards) {
            card.setSelected(false);
        }

        homeBase.reset();
        waveManager.startWave(1);
        enemies.clear();
        enemyProjectiles.clear();
        playerArrows.clear();
        collectibles.clear();
        combatSystem.ResetDefeatedEnemyCount();

        meleePlayer.init(120.0f, gameGrid.getLaneCenterY(0));
        rangedPlayer.init(120.0f, gameGrid.getLaneCenterY(1));
        defensePlayer.init(120.0f, gameGrid.getLaneCenterY(2));
        };

    restartGame();

    // Input state tracking
    bool prevMouseLeft = false;
    bool prevEscKey = false;
    bool prevSpaceKey = false;

    double lastTime = glfwGetTime();

    // ==========================================
    // MAIN GAME LOOP
    // ==========================================
    while (!gameWindow.shouldClose() && !stateMachine.WantsExit()) {
        double currentTime = glfwGetTime();
        float dt = static_cast<float>(currentTime - lastTime);
        if (dt > 0.1f)
            dt = 0.1f;
        lastTime = currentTime;

        if (stateMachine.WantsRestart()) {
            stateMachine.ClearRestart();
            restartGame();
        }

        double mouseX = InputManager::getMouseX();
        double mouseY = InputManager::getMouseY();
        glm::vec2 mousePos(static_cast<float>(mouseX), static_cast<float>(mouseY));

        bool mouseLeftDown =
            InputManager::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
        bool mouseClicked = (mouseLeftDown && !prevMouseLeft);
        prevMouseLeft = mouseLeftDown;

        bool escDown = InputManager::isKeyPressed(GLFW_KEY_ESCAPE);
        bool escPressed = (escDown && !prevEscKey);
        prevEscKey = escDown;

        bool spaceDown = InputManager::isKeyPressed(GLFW_KEY_SPACE);
        bool spacePressed = (spaceDown && !prevSpaceKey);
        prevSpaceKey = spaceDown;

        // Toggle pause on ESC
        if (escPressed && (stateMachine.GetState() == GameState::GAMEPLAY ||
            stateMachine.GetState() == GameState::PAUSED)) {
            stateMachine.TogglePause();
        }

        camera.Update(dt);

        // ==========================================
        // STATE UPDATES
        // ==========================================
        if (stateMachine.GetState() == GameState::CUTSCENE) {
            cutscene.Update(dt, &camera, spacePressed);
            if (cutscene.IsFinished()) {
                stateMachine.SetState(GameState::GAMEPLAY);
            }
        }
        else if (stateMachine.GetState() == GameState::GAMEPLAY) {
            // 0. Update Player Cards & Handle Selection
            for (size_t i = 0; i < unitCards.size(); ++i) {
                unitCards[i].update(economy, mouseX, mouseY);

                if (unitCards[i].handleMouseClick(mouseX, mouseY, mouseLeftDown)) {
                    // Radio-button behavior: selecting one card deselects the others
                    if (unitCards[i].isSelected()) {
                        for (size_t j = 0; j < unitCards.size(); ++j) {
                            if (i != j) {
                                unitCards[j].setSelected(false);
                            }
                        }
                    }
                }
            }

            // 1. Lane Selection (hover and click)
            laneSelector.update(gameGrid, mouseX, mouseY, mouseClicked);

            // 1B. UNIT DEPLOYMENT / RECRUITMENT
            PlayerCardUI* selectedCard = nullptr;
            for (auto& card : unitCards) {
                if (card.isSelected()) {
                    selectedCard = &card;
                    break;
                }
            }

            // If a card is selected and player clicks inside a lane on the playfield (Y > 50)
            if (selectedCard && mouseClicked && mouseY > 50.0f && laneSelector.getHoveredLaneId() != -1) {
                int targetLane = laneSelector.getHoveredLaneId();
                float targetY = gameGrid.getLaneCenterY(targetLane);

                // Deduct cost and place unit
                if (economy.spendCoins(selectedCard->getCost())) {
                    if (selectedCard->getUnitType() == CardUnitType::Melee) {
                        meleePlayer.init(120.0f, targetY);
                    }
                    else if (selectedCard->getUnitType() == CardUnitType::Ranged) {
                        rangedPlayer.init(120.0f, targetY);
                    }
                    else if (selectedCard->getUnitType() == CardUnitType::Defense) {
                        defensePlayer.init(120.0f, targetY);
                    }
                }

                // Deselect card once placement completes
                selectedCard->setSelected(false);
            }

            // 2. ActionManager handles W (Jump) and S (Duck) for selected lanes
            std::vector<int> selectedLanes;
            if (laneSelector.getSelectedLaneId() != -1) {
                selectedLanes.push_back(laneSelector.getSelectedLaneId());
            }
            ActionManager::handleInput(activePlayers, selectedLanes);

            // Test key to simulate taking damage (K key)
            static bool wasKPressed = false;
            bool isKPressed = InputManager::isKeyPressed(GLFW_KEY_K);
            if (isKPressed && !wasKPressed) {
                for (Player* p : activePlayers) {
                    if (p && p->isActive() && p->getLaneId() == laneSelector.getSelectedLaneId()) {
                        p->takeDamage(1);
                    }
                }
            }
            wasKPressed = isKPressed;

            // 3. Update Players
            meleePlayer.update(dt);
            rangedPlayer.update(dt);
            defensePlayer.update(dt);

            // 4. Fire Arrow on Spacebar tap
            if (spacePressed && rangedPlayer.isAlive()) {
                Projectile arrow;
                arrow.init(rangedPlayer.getPosition().x + 35.0f,
                    rangedPlayer.getPosition().y + 24.0f);
                playerArrows.push_back(arrow);
            }

            // 5. Wave Manager Spawning
            EnemyType spawnType;
            int spawnLane = 0;
            if (waveManager.shouldSpawnEnemy(dt, gameGrid.getLaneCount(), spawnType,
                spawnLane)) {
                float startY = gameGrid.getLaneCenterY(spawnLane);
                glm::vec2 spawnPos(780.0f, startY);

                if (spawnType == EnemyType::EasyUpperRanged) {
                    enemies.push_back(std::make_unique<EasyEnemy>(spawnLane, spawnPos));
                }
                else if (spawnType == EnemyType::MediumLowerRanged) {
                    enemies.push_back(std::make_unique<MediumEnemy>(spawnLane, spawnPos));
                }
                else if (spawnType == EnemyType::HardMelee) {
                    enemies.push_back(std::make_unique<HardEnemy>(spawnLane, spawnPos));
                }
            }

            // Check if wave enemies are cleared
            if (waveManager.isSpawningFinished() && enemies.empty()) {
                waveManager.onWaveEnemiesCleared();
                if (waveManager.isGameWon()) {
                    stateMachine.TriggerVictory();
                }
            }

            // 6. Update Enemies & Hostile Projectiles
            for (auto& e : enemies) {
                if (e)
                    e->Update(dt, enemyProjectiles);
            }
            for (auto& ep : enemyProjectiles) {
                ep.Update(dt);
            }
            for (auto& arrow : playerArrows) {
                if (arrow.isActive())
                    arrow.update();
            }
            for (auto& col : collectibles) {
                col.Update(dt);
            }

            // 7. Collision & Combat Systems
            if (collisionSystem.CheckHomeBaseBreach(enemies, homeBase)) {
                stateMachine.TriggerDefeat();
            }

            // Check Unit Encounters
            collisionSystem.CheckPlayerEnemyEncounters(activePlayers, enemies);

            // Check Player Arrow Collisions
            collisionSystem.CheckPlayerArrows(playerArrows, enemies);

            // Check Enemy Projectile Collisions
            int preHealth = homeBase.getHealth();
            collisionSystem.CheckEnemyProjectiles(enemyProjectiles, activePlayers,
                homeBase);
            if (homeBase.getHealth() < preHealth) {
                camera.TriggerShake(0.2f, 3.5f);
            }

            if (homeBase.hasLostGame()) {
                stateMachine.TriggerDefeat();
            }

            // Check Collectibles & Sync with Economy
            int collectedGold = economy.getCoins();
            collisionSystem.CheckPlayerCollectiblePickups(activePlayers, collectibles,
                collectedGold);
            economy.setCoins(collectedGold);

            // Run Damage Ticks & Drops
            combatSystem.Update(dt, activePlayers, enemies, collectibles);

            // Cleanup inactive objects
            enemyProjectiles.erase(
                std::remove_if(enemyProjectiles.begin(), enemyProjectiles.end(),
                    [](const EnemyProjectile& ep) {
                        return !ep.IsActive();
                    }),
                enemyProjectiles.end());
            playerArrows.erase(
                std::remove_if(playerArrows.begin(), playerArrows.end(),
                    [](const Projectile& pr) { return !pr.isActive(); }),
                playerArrows.end());
            collectibles.erase(
                std::remove_if(collectibles.begin(), collectibles.end(),
                    [](const Collectible& cl) { return !cl.IsActive(); }),
                collectibles.end());
        }
        else {
            stateMachine.Update(dt, mousePos, mouseClicked);
            if (stateMachine.GetState() == GameState::CUTSCENE) {
                cutscene.Start();
            }
        }

        // ==========================================
        // RENDER PASS
        // ==========================================
        glClearColor(0.10f, 0.12f, 0.18f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (stateMachine.GetState() == GameState::CUTSCENE) {
            cutscene.Render(renderer);
        }
        else if (stateMachine.GetState() == GameState::MAIN_MENU ||
            stateMachine.GetState() == GameState::SOUND_SETTINGS) {
            stateMachine.RenderMenus(renderer);
        }
        else {
            // 1. Grid & Castle Environment
            renderer.Draw(gridVAO, glm::vec2(0.0f), glm::vec2(1.0f));
            renderer.Draw(envVAO, glm::vec2(0.0f), glm::vec2(1.0f));

            // 2. Full Lane Highlights
            const auto& lanes = gameGrid.getLanes();
            for (const auto& lane : lanes) {
                glm::vec2 pos(lane.minX, lane.minY);
                glm::vec2 size(lane.maxX - lane.minX, lane.maxY - lane.minY);

                if (lane.id == laneSelector.getSelectedLaneId()) {
                    renderer.DrawQuad(pos, size, glm::vec4(0.20f, 0.24f, 0.32f, 0.60f));
                    renderer.DrawQuad(glm::vec2(lane.minX, lane.minY),
                        glm::vec2(size.x, 2.5f),
                        glm::vec4(0.95f, 0.80f, 0.25f, 0.90f));
                    renderer.DrawQuad(glm::vec2(lane.minX, lane.maxY - 2.5f),
                        glm::vec2(size.x, 2.5f),
                        glm::vec4(0.95f, 0.80f, 0.25f, 0.90f));
                }
                else if (lane.id == laneSelector.getHoveredLaneId()) {
                    renderer.DrawQuad(pos, size, glm::vec4(0.16f, 0.18f, 0.22f, 0.52f));
                    renderer.DrawQuad(glm::vec2(lane.minX, lane.minY),
                        glm::vec2(size.x, 1.8f),
                        glm::vec4(0.35f, 0.38f, 0.44f, 0.75f));
                    renderer.DrawQuad(glm::vec2(lane.minX, lane.maxY - 1.8f),
                        glm::vec2(size.x, 1.8f),
                        glm::vec4(0.35f, 0.38f, 0.44f, 0.75f));
                }
            }

            // 3. Home Base (Castle Citadel Gate on the Left)
            glm::vec4 towerColor =
                homeBase.hasLostGame()
                ? glm::vec4(0.65f, 0.15f, 0.15f, 1.0f)
                : glm::vec4(0.32f, 0.34f, 0.40f, 1.0f);
            renderer.DrawQuad(homeBase.getPosition(), homeBase.getSize(), towerColor);

            // Battlements
            for (int b = 0; b < 6; ++b) {
                renderer.DrawQuad(glm::vec2(78.0f, 75.0f + b * 75.0f),
                    glm::vec2(20.0f, 35.0f),
                    glm::vec4(0.40f, 0.42f, 0.48f, 1.0f));
            }
            // Iron Portcullis Gate
            renderer.DrawQuad(glm::vec2(25.0f, 470.0f), glm::vec2(50.0f, 70.0f),
                glm::vec4(0.18f, 0.18f, 0.22f, 1.0f));

            // Castle Health Bar & Label
            renderer.DrawString("CASTLE HP", glm::vec2(8.0f, 68.0f), 0.82f,
                glm::vec4(1.0f, 0.90f, 0.30f, 1.0f));
            renderer.DrawHealthBar(glm::vec2(8.0f, 82.0f), glm::vec2(82.0f, 9.0f),
                homeBase.getHealth(), homeBase.getMaxHealth(),
                true);
            std::string castleHpNum = std::to_string(homeBase.getHealth()) + " / " +
                std::to_string(homeBase.getMaxHealth());
            renderer.DrawString(castleHpNum, glm::vec2(26.0f, 94.0f), 0.75f,
                glm::vec4(0.95f, 0.95f, 0.95f, 1.0f));

            // Elevated Balcony for Princess Aurelia
            glm::vec2 balconyPos(24.0f, 175.0f);
            renderer.DrawQuad(balconyPos + glm::vec2(0.0f, 28.0f),
                glm::vec2(52.0f, 25.0f),
                glm::vec4(0.38f, 0.40f, 0.46f, 1.0f));
            renderer.DrawQuad(balconyPos + glm::vec2(-4.0f, 26.0f),
                glm::vec2(60.0f, 4.0f),
                glm::vec4(0.85f, 0.70f, 0.20f, 1.0f));
            renderer.DrawQuad(balconyPos + glm::vec2(4.0f, 38.0f),
                glm::vec2(44.0f, 52.0f),
                glm::vec4(0.55f, 0.12f, 0.60f, 1.0f));
            renderer.DrawQuad(balconyPos + glm::vec2(17.0f, 50.0f),
                glm::vec2(18.0f, 20.0f),
                glm::vec4(0.95f, 0.85f, 0.25f, 1.0f));

            // Princess Aurelia
            glm::vec2 princessPos = balconyPos + glm::vec2(26.0f, 16.0f);
            float pBob = sinf(static_cast<float>(currentTime) * 3.0f) * 1.5f;
            princessPos.y += pBob;

            renderer.DrawQuad(princessPos + glm::vec2(-9.0f, 0.0f),
                glm::vec2(18.0f, 28.0f),
                glm::vec4(0.68f, 0.25f, 0.75f, 1.0f));
            renderer.DrawQuad(princessPos + glm::vec2(-7.0f, -2.0f),
                glm::vec2(14.0f, 10.0f),
                glm::vec4(0.95f, 0.80f, 0.25f, 1.0f));
            renderer.DrawQuad(princessPos + glm::vec2(-5.0f, -14.0f),
                glm::vec2(10.0f, 12.0f),
                glm::vec4(0.98f, 0.85f, 0.75f, 1.0f));
            renderer.DrawQuad(princessPos + glm::vec2(-8.0f, -16.0f),
                glm::vec2(4.0f, 18.0f),
                glm::vec4(0.95f, 0.85f, 0.35f, 1.0f));
            renderer.DrawQuad(princessPos + glm::vec2(4.0f, -16.0f),
                glm::vec2(4.0f, 18.0f),
                glm::vec4(0.95f, 0.85f, 0.35f, 1.0f));
            renderer.DrawTriangle(princessPos + glm::vec2(-5.0f, -15.0f),
                princessPos + glm::vec2(5.0f, -15.0f),
                princessPos + glm::vec2(0.0f, -22.0f),
                glm::vec4(1.0f, 0.85f, 0.20f, 1.0f));
            renderer.DrawQuad(princessPos + glm::vec2(-3.0f, 5.0f),
                glm::vec2(6.0f, 4.0f),
                glm::vec4(0.98f, 0.85f, 0.75f, 1.0f));

            // 4. Render Collectibles
            for (auto& col : collectibles) {
                col.Render(renderer);
            }

            // 5. Render Players
            for (Player* p : activePlayers) {
                if (p && p->isActive()) {
                    // Render player model with increasing red tint
                    VertexArray* vao = (p == &meleePlayer) ? &meleeVAO : (p == &rangedPlayer ? &rangedVAO : &defenseVAO);
                    myRenderer.Draw(*vao, p->getRenderPosition(), glm::vec2(1.0f, 1.0f), 0.0f, p->getDamageTint());

                    // Render health bar above player (width = 55.0f)
                    HealthUI::draw(myRenderer, quadVAO, p->getRenderPosition(), 55.0f, p->getHealth(), p->getMaxHealth());
                }
            }

            // 6. Render Player Arrows
            for (auto& arrow : playerArrows) {
                if (arrow.isActive()) {
                    renderer.Draw(projectileVAO, arrow.getPosition(),
                        glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));
                }
            }

            // 7. Render Enemies & Hostile Projectiles
            for (auto& e : enemies) {
                if (e)
                    e->Render(renderer);
            }
            for (auto& ep : enemyProjectiles) {
                ep.Render(renderer);
            }

            // 8. HUD Header (Gold, Wave, Unit Cards, Hints)
            renderer.DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(screenWidth, 50.0f),
                glm::vec4(0.08f, 0.10f, 0.15f, 0.90f));
            renderer.DrawQuad(glm::vec2(0.0f, 48.0f), glm::vec2(screenWidth, 2.0f),
                glm::vec4(0.85f, 0.70f, 0.20f, 1.0f));

            // Current Gold from Economy
            std::string goldStr = "GOLD: " + std::to_string(economy.getCoins());
            renderer.DrawString(goldStr, glm::vec2(14.0f, 16.0f), 1.15f,
                glm::vec4(1.0f, 0.88f, 0.25f, 1.0f));

            // Current Wave
            std::string waveStr =
                "WAVE: " + std::to_string(waveManager.getCurrentWave()) + "/3";
            renderer.DrawString(waveStr, glm::vec2(138.0f, 16.0f), 1.15f,
                glm::vec4(0.90f, 0.90f, 0.95f, 1.0f));

            // Render Player Selection Cards in HUD
            for (const auto& card : unitCards) {
                card.render(renderer);
            }

            // Control Hints
            renderer.DrawString("[SPACE] SHOOT   [W/S] JUMP/DUCK",
                glm::vec2(520.0f, 18.0f), 0.72f,
                glm::vec4(0.70f, 0.75f, 0.85f, 1.0f));

            // 9. Menus Overlay (Pause / Win / Lose)
            if (stateMachine.IsPaused() ||
                stateMachine.GetState() == GameState::GAME_OVER_WIN ||
                stateMachine.GetState() == GameState::GAME_OVER_LOSE) {
                stateMachine.RenderMenus(renderer);
            }
        }

        gameWindow.update();
    }

    return 0;
}