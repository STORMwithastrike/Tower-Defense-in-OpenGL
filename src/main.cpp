#include "../headers/Window.h"
#include "../headers/InputManager.h"
#include "../headers/Grid.h"
#include "../headers/Environment.h"
#include "../headers/HomeBase.h"
#include "../headers/LaneSelector.h"
#include "../headers/ActionManager.h"
#include "../headers/Player.h"
#include "../headers/MeleePlayer.h"
#include "../headers/RangedPlayer.h"
#include "../headers/DefensePlayer.h"
#include "../headers/Projectile.h"
#include "../headers/WaveManager.h"

#include "render/Shader.h"
#include "render/Camera.h"
#include "render/Renderer.h"
#include "render/VertexArray.h"

#include "core/GameStateMachine.h"
#include "core/CutsceneManager.h"

#include "game/CollisionSystem.h"
#include "game/CombatSystem.h"
#include "game/EasyEnemy.h"
#include "game/MediumEnemy.h"
#include "game/HardEnemy.h"
#include "game/EnemyProjectile.h"
#include "game/Collectible.h"

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>

int main() {
    const float screenWidth = 800.0f;
    const float screenHeight = 600.0f;

    Window gameWindow(static_cast<int>(screenWidth), static_cast<int>(screenHeight));
    if (!gameWindow.initialize("Castle Defense - Protect the Princess")) {
        std::cerr << "Failed to initialize Game Window!" << std::endl;
        return -1;
    }

    GLFWwindow* rawWindow = gameWindow.getGLFWWindow();
    glfwSetKeyCallback(rawWindow, InputManager::keyCallback);
    glfwSetMouseButtonCallback(rawWindow, InputManager::mouseButtonCallback);
    glfwSetCursorPosCallback(rawWindow, InputManager::cursorPosCallback);

    // 1. Shaders, Camera & Renderer
    Shader mainShader(ASSETS_DIR "shaders/main.vert", ASSETS_DIR "shaders/main.frag");
    Camera camera(screenWidth, screenHeight);
    Renderer renderer(&mainShader, screenWidth, screenHeight);
    renderer.SetCamera(&camera);

    // 2. Playfield Grid & Environment
    Grid gameGrid;
    gameGrid.init();
    VertexArray gridVAO(gameGrid.getVertices(), gameGrid.getIndices());

    Environment environment;
    environment.init();
    VertexArray envVAO(environment.getVertices(), environment.getIndices());

    // 3. Home Base & Lane Selector
    HomeBase homeBase(100.0f, 100.0f, 10.0f, 440.0f);
    LaneSelector laneSelector;

    // 4. State Machine & Cutscene
    GameStateMachine stateMachine(screenWidth, screenHeight);
    CutsceneManager cutscene(screenWidth, screenHeight);

    // 5. Wave & Combat Systems
    WaveManager waveManager;
    CollisionSystem collisionSystem;
    CombatSystem combatSystem;

    // 6. Basic 1x1 Quad for highlights & Base
    std::vector<float> quadVertices = {
        0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f
    };
    std::vector<unsigned int> quadIndices = { 0, 1, 2, 2, 3, 0 };
    VertexArray quadVAO(quadVertices, quadIndices);

    // 7. Players (Friendly Blue Defenders)
    MeleePlayer meleePlayer(0);
    meleePlayer.init(120.0f, gameGrid.getLaneCenterY(0));
    VertexArray meleeVAO(meleePlayer.getVertices(), meleePlayer.getIndices());

    RangedPlayer rangedPlayer(1);
    rangedPlayer.init(120.0f, gameGrid.getLaneCenterY(1));
    VertexArray rangedVAO(rangedPlayer.getVertices(), rangedPlayer.getIndices());

    DefensePlayer defensePlayer(2);
    defensePlayer.init(120.0f, gameGrid.getLaneCenterY(2));
    VertexArray defenseVAO(defensePlayer.getVertices(), defensePlayer.getIndices());

    std::vector<Player*> activePlayers = { &meleePlayer, &rangedPlayer, &defensePlayer };

    // Player Projectiles
    std::vector<Projectile> playerArrows;
    Projectile arrowProto;
    arrowProto.init(0.0f, 0.0f);
    VertexArray projectileVAO(arrowProto.getVertices(), arrowProto.getIndices());

    // Enemy Entities, Hostile Projectiles & Collectibles
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<EnemyProjectile> enemyProjectiles;
    std::vector<Collectible> collectibles;

    int totalCoins = 30; // Starts with enough for at least 3 Melee units

    // Reset helper
    auto restartGame = [&]() {
        totalCoins = 30;
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
        if (dt > 0.1f) dt = 0.1f;
        lastTime = currentTime;

        if (stateMachine.WantsRestart()) {
            stateMachine.ClearRestart();
            restartGame();
        }

        double mouseX = InputManager::getMouseX();
        double mouseY = InputManager::getMouseY();
        glm::vec2 mousePos(static_cast<float>(mouseX), static_cast<float>(mouseY));

        bool mouseLeftDown = InputManager::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
        bool mouseClicked = (mouseLeftDown && !prevMouseLeft);
        prevMouseLeft = mouseLeftDown;

        bool escDown = InputManager::isKeyPressed(GLFW_KEY_ESCAPE);
        bool escPressed = (escDown && !prevEscKey);
        prevEscKey = escDown;

        bool spaceDown = InputManager::isKeyPressed(GLFW_KEY_SPACE);
        bool spacePressed = (spaceDown && !prevSpaceKey);
        prevSpaceKey = spaceDown;

        // Toggle pause on ESC
        if (escPressed && (stateMachine.GetState() == GameState::GAMEPLAY || stateMachine.GetState() == GameState::PAUSED)) {
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
            // 1. Lane Selection
            laneSelector.update(gameGrid, mouseX, mouseY, mouseClicked);

            // 2. ActionManager handles W (Jump) and S (Duck) for selected lanes
            std::vector<int> selectedLanes;
            if (laneSelector.getSelectedLaneId() != -1) {
                selectedLanes.push_back(laneSelector.getSelectedLaneId());
            }
            ActionManager::handleInput(activePlayers, selectedLanes);

            // 3. Update Players
            meleePlayer.update(dt);
            rangedPlayer.update(dt);
            defensePlayer.update(dt);

            // 4. Fire Arrow on Spacebar tap
            if (spacePressed && rangedPlayer.isAlive()) {
                Projectile arrow;
                arrow.init(rangedPlayer.getPosition().x + 35.0f, rangedPlayer.getPosition().y + 24.0f);
                playerArrows.push_back(arrow);
            }

            // 5. Wave Manager Spawning
            EnemyType spawnType;
            int spawnLane = 0;
            if (waveManager.shouldSpawnEnemy(dt, gameGrid.getLaneCount(), spawnType, spawnLane)) {
                float startY = gameGrid.getLaneCenterY(spawnLane);
                glm::vec2 spawnPos(780.0f, startY);

                if (spawnType == EnemyType::EasyUpperRanged) {
                    enemies.push_back(std::make_unique<EasyEnemy>(spawnLane, spawnPos));
                } else if (spawnType == EnemyType::MediumLowerRanged) {
                    enemies.push_back(std::make_unique<MediumEnemy>(spawnLane, spawnPos));
                } else if (spawnType == EnemyType::HardMelee) {
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
                if (e) e->Update(dt, enemyProjectiles);
            }
            for (auto& ep : enemyProjectiles) {
                ep.Update(dt);
            }
            for (auto& arrow : playerArrows) {
                if (arrow.isActive()) arrow.update();
            }
            for (auto& col : collectibles) {
                col.Update(dt);
            }

            // 7. Collision & Combat Systems
            // Home Base Breach Check
            for (const auto& e : enemies) {
                if (e && !e->IsDead() && homeBase.checkEnemyReached(e->GetPosition().x)) {
                    stateMachine.TriggerDefeat();
                    break;
                }
            }

            // Check Unit Encounters
            collisionSystem.CheckPlayerEnemyEncounters(activePlayers, enemies);

            // Check Player Arrow Collisions
            collisionSystem.CheckPlayerArrows(playerArrows, enemies);

            // Check Enemy Projectile Collisions (with ducking/jumping dodging)
            collisionSystem.CheckEnemyProjectiles(enemyProjectiles, activePlayers);

            // Check Collectibles
            collisionSystem.CheckPlayerCollectiblePickups(activePlayers, collectibles, totalCoins);

            // Run Damage Ticks & Drops
            combatSystem.Update(dt, activePlayers, enemies, collectibles);

            // Cleanup inactive objects
            enemyProjectiles.erase(std::remove_if(enemyProjectiles.begin(), enemyProjectiles.end(),
                [](const EnemyProjectile& ep) { return !ep.IsActive(); }), enemyProjectiles.end());
            playerArrows.erase(std::remove_if(playerArrows.begin(), playerArrows.end(),
                [](const Projectile& pr) { return !pr.isActive(); }), playerArrows.end());
            collectibles.erase(std::remove_if(collectibles.begin(), collectibles.end(),
                [](const Collectible& cl) { return !cl.IsActive(); }), collectibles.end());
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
            // 1. Grid & Environment
            renderer.Draw(gridVAO, glm::vec2(0.0f), glm::vec2(1.0f));
            renderer.Draw(envVAO, glm::vec2(0.0f), glm::vec2(1.0f));

            // 2. Home Base
            glm::vec4 baseColor = homeBase.hasLostGame()
                ? glm::vec4(0.85f, 0.15f, 0.15f, 1.0f)
                : glm::vec4(0.18f, 0.35f, 0.75f, 1.0f);
            renderer.Draw(quadVAO, homeBase.getPosition(), homeBase.getSize(), 0.0f, baseColor);

            // 3. Lane Highlights
            const auto& lanes = gameGrid.getLanes();
            for (const auto& lane : lanes) {
                float highlightAmount = 0.0f;
                glm::vec4 tintColor(1.0f);

                if (lane.id == laneSelector.getSelectedLaneId()) {
                    highlightAmount = 1.0f;
                    tintColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
                } else if (lane.id == laneSelector.getHoveredLaneId()) {
                    highlightAmount = 1.0f;
                    tintColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
                }

                if (highlightAmount > 0.0f) {
                    glm::vec2 pos(lane.minX, lane.minY);
                    glm::vec2 size(lane.maxX - lane.minX, lane.maxY - lane.minY);
                    renderer.Draw(quadVAO, pos, size, highlightAmount, tintColor);
                }
            }

            // 4. Render Collectibles
            for (auto& col : collectibles) {
                col.Render(renderer);
            }

            // 5. Render Players
            if (meleePlayer.isAlive()) {
                renderer.Draw(meleeVAO, meleePlayer.getRenderPosition(), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));
            }
            if (rangedPlayer.isAlive()) {
                renderer.Draw(rangedVAO, rangedPlayer.getRenderPosition(), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));
            }
            if (defensePlayer.isAlive()) {
                renderer.Draw(defenseVAO, defensePlayer.getRenderPosition(), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));
            }

            // 6. Render Player Arrows
            for (auto& arrow : playerArrows) {
                if (arrow.isActive()) {
                    renderer.Draw(projectileVAO, arrow.getPosition(), glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));
                }
            }

            // 7. Render Enemies & Enemy Hostile Projectiles
            for (auto& e : enemies) {
                if (e) e->Render(renderer);
            }
            for (auto& ep : enemyProjectiles) {
                ep.Render(renderer);
            }

            // 8. HUD Header (Gold, Wave, Hints)
            renderer.DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(screenWidth, 50.0f),
                              glm::vec4(0.08f, 0.10f, 0.15f, 0.90f));
            renderer.DrawQuad(glm::vec2(0.0f, 48.0f), glm::vec2(screenWidth, 2.0f),
                              glm::vec4(0.85f, 0.70f, 0.20f, 1.0f));

            std::string goldStr = "GOLD: " + std::to_string(totalCoins);
            renderer.DrawString(goldStr, glm::vec2(20.0f, 16.0f), 1.25f, glm::vec4(1.0f, 0.88f, 0.25f, 1.0f));

            std::string waveStr = "WAVE: " + std::to_string(waveManager.getCurrentWave()) + " / 3";
            renderer.DrawString(waveStr, glm::vec2(220.0f, 16.0f), 1.25f, glm::vec4(0.90f, 0.90f, 0.95f, 1.0f));

            renderer.DrawString("[SPACE] SHOOT   [W/S] JUMP/DUCK   [ESC] PAUSE",
                                glm::vec2(400.0f, 18.0f), 0.82f, glm::vec4(0.70f, 0.75f, 0.85f, 1.0f));

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
