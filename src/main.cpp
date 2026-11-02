#include "../headers/Window.h"
#include "../headers/InputManager.h"
#include "../headers/Grid.h"
#include "../headers/Environment.h"

#include "render/Shader.h"
#include "render/Camera.h"
#include "render/Renderer.h"
#include "render/VertexArray.h"

#include "core/GameStateMachine.h"
#include "core/CutsceneManager.h"

#include "game/CollisionSystem.h"
#include "game/CombatSystem.h"
#include "game/MeleePlayer.h"
#include "game/EasyEnemy.h"
#include "game/MediumEnemy.h"
#include "game/HardEnemy.h"

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

    // 1. Shaders & Camera
    Shader mainShader(ASSETS_DIR "shaders/main.vert", ASSETS_DIR "shaders/main.frag");
    Camera camera(screenWidth, screenHeight);
    Renderer renderer(&mainShader, screenWidth, screenHeight);
    renderer.SetCamera(&camera);

    // 2. Playfield Grid & Castle Environment
    Grid grid;
    grid.init();
    VertexArray gridVAO(grid.getVertices(), grid.getIndices());

    Environment environment;
    environment.init();
    VertexArray envVAO(environment.getVertices(), environment.getIndices());

    // 3. State Machine & Cutscene
    GameStateMachine stateMachine(screenWidth, screenHeight);
    CutsceneManager cutscene(screenWidth, screenHeight);

    // 4. Combat & Collision Systems
    CollisionSystem collisionSystem;
    CombatSystem combatSystem;

    // 5. Game State Data
    int totalCoins = 30; // Starts with enough for at least 3 Melee Players
    int currentWave = 1;
    float waveTimer = 0.0f;
    int enemiesSpawnedInWave = 0;
    bool waveActive = true;
    const float homeBaseMaxX = 100.0f;

    std::vector<std::unique_ptr<Player>> players;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<Projectile> projectiles;
    std::vector<Collectible> collectibles;

    // Lane Selection Tracking (4 lanes)
    std::vector<bool> selectedLanes(grid.getLaneCount(), false);

    // Reset / Restart Helper
    auto restartGame = [&]() {
        totalCoins = 30;
        currentWave = 1;
        waveTimer = 0.0f;
        enemiesSpawnedInWave = 0;
        waveActive = true;
        players.clear();
        enemies.clear();
        projectiles.clear();
        collectibles.clear();
        for (size_t i = 0; i < selectedLanes.size(); ++i) selectedLanes[i] = false;
        combatSystem.ResetDefeatedEnemyCount();

        // Spawn starter Melee Blue Knight on Lane 0
        float y0 = grid.getLaneCenterY(0);
        players.push_back(std::make_unique<MeleePlayer>(0, glm::vec2(130.0f, y0)));
    };

    restartGame();

    // Input Edge-Detection State
    bool prevMouseLeft = false;
    bool prevEscKey = false;
    bool prevKeyW = false;
    bool prevKeyS = false;
    bool prevKey1 = false;
    bool prevKeySpace = false;

    double lastTime = glfwGetTime();

    // ==========================================
    // MAIN APPLICATION LOOP
    // ==========================================
    while (!gameWindow.shouldClose() && !stateMachine.WantsExit()) {
        double currentTime = glfwGetTime();
        float dt = static_cast<float>(currentTime - lastTime);
        if (dt > 0.1f) dt = 0.1f; // Clamp delta time to avoid physics tunneling
        lastTime = currentTime;

        // Check for restart request from pause / win / lose menus
        if (stateMachine.WantsRestart()) {
            stateMachine.ClearRestart();
            restartGame();
        }

        // Mouse & Input Coordinates
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
        bool spacePressed = (spaceDown && !prevKeySpace);
        prevKeySpace = spaceDown;

        bool keyWDown = InputManager::isKeyPressed(GLFW_KEY_W) || InputManager::isKeyPressed(GLFW_KEY_UP);
        bool keyWPressed = (keyWDown && !prevKeyW);
        prevKeyW = keyWDown;

        bool keySDown = InputManager::isKeyPressed(GLFW_KEY_S) || InputManager::isKeyPressed(GLFW_KEY_DOWN);
        bool keySPressed = (keySDown && !prevKeyS);
        prevKeyS = keySDown;

        bool key1Down = InputManager::isKeyPressed(GLFW_KEY_1);
        bool key1Pressed = (key1Down && !prevKey1);
        prevKey1 = key1Down;

        // ESC toggles pause during gameplay
        if (escPressed && (stateMachine.GetState() == GameState::GAMEPLAY || stateMachine.GetState() == GameState::PAUSED)) {
            stateMachine.TogglePause();
        }

        // Update Camera shake
        camera.Update(dt);

        // ==========================================
        // UPDATE LOGIC BASED ON CURRENT GAME STATE
        // ==========================================
        if (stateMachine.GetState() == GameState::CUTSCENE) {
            cutscene.Update(dt, &camera, spacePressed);
            if (cutscene.IsFinished()) {
                stateMachine.SetState(GameState::GAMEPLAY);
            }
        }
        else if (stateMachine.GetState() == GameState::GAMEPLAY) {
            // 1. Mouse Lane Hover & Toggle Selection
            int hoveredLane = -1;
            const auto& lanes = grid.getLanes();
            for (int i = 0; i < static_cast<int>(lanes.size()); ++i) {
                if (mouseY >= lanes[i].minY && mouseY <= lanes[i].maxY &&
                    mouseX >= lanes[i].minX && mouseX <= lanes[i].maxX) {
                    hoveredLane = i;
                    break;
                }
            }

            if (mouseClicked && hoveredLane != -1) {
                // Clicking toggles lane between selected (highlighted) and deselected
                // Multiple lanes can be selected at once!
                selectedLanes[hoveredLane] = !selectedLanes[hoveredLane];
            }

            // 2. Dodging Controls for All Selected Lanes
            // W / Up Arrow: Every player in all selected lanes jumps (avoids low attacks)
            if (keyWPressed) {
                for (auto& p : players) {
                    if (p && selectedLanes[p->GetLane()]) {
                        p->Jump();
                    }
                }
            }
            // S / Down Arrow: Every player in all selected lanes ducks (avoids high attacks)
            if (keySPressed) {
                for (auto& p : players) {
                    if (p && selectedLanes[p->GetLane()]) {
                        p->Duck();
                    }
                }
            }

            // 3. Player Placement Shortcut (Press '1' to spawn Blue Knight on first selected lane)
            if (key1Pressed && totalCoins >= 10) {
                for (int i = 0; i < static_cast<int>(selectedLanes.size()); ++i) {
                    if (selectedLanes[i]) {
                        totalCoins -= 10;
                        players.push_back(std::make_unique<MeleePlayer>(i, glm::vec2(120.0f, grid.getLaneCenterY(i))));
                        break;
                    }
                }
            }

            // 4. Wave Spawner Logic (Waves 1, 2, 3)
            waveTimer += dt;
            if (waveActive) {
                if (currentWave == 1) {
                    // Wave 1: Minimum 3 Easy enemies (Upper Ranged - Arbalist)
                    if (enemiesSpawnedInWave < 3 && waveTimer >= 2.5f) {
                        waveTimer = 0.0f;
                        int rLane = rand() % grid.getLaneCount();
                        enemies.push_back(std::make_unique<EasyEnemy>(rLane, glm::vec2(780.0f, grid.getLaneCenterY(rLane))));
                        enemiesSpawnedInWave++;
                    }
                } else if (currentWave == 2) {
                    // Wave 2: Minimum 5 enemies (Mix of Easy & Medium)
                    if (enemiesSpawnedInWave < 5 && waveTimer >= 2.2f) {
                        waveTimer = 0.0f;
                        int rLane = rand() % grid.getLaneCount();
                        if (enemiesSpawnedInWave % 2 == 0) {
                            enemies.push_back(std::make_unique<EasyEnemy>(rLane, glm::vec2(780.0f, grid.getLaneCenterY(rLane))));
                        } else {
                            enemies.push_back(std::make_unique<MediumEnemy>(rLane, glm::vec2(780.0f, grid.getLaneCenterY(rLane))));
                        }
                        enemiesSpawnedInWave++;
                    }
                } else if (currentWave == 3) {
                    // Wave 3: Minimum 10 enemies (Mix of Easy, Medium, and Hard)
                    if (enemiesSpawnedInWave < 10 && waveTimer >= 1.8f) {
                        waveTimer = 0.0f;
                        int rLane = rand() % grid.getLaneCount();
                        int pick = enemiesSpawnedInWave % 3;
                        if (pick == 0) {
                            enemies.push_back(std::make_unique<EasyEnemy>(rLane, glm::vec2(780.0f, grid.getLaneCenterY(rLane))));
                        } else if (pick == 1) {
                            enemies.push_back(std::make_unique<MediumEnemy>(rLane, glm::vec2(780.0f, grid.getLaneCenterY(rLane))));
                        } else {
                            enemies.push_back(std::make_unique<HardEnemy>(rLane, glm::vec2(780.0f, grid.getLaneCenterY(rLane))));
                        }
                        enemiesSpawnedInWave++;
                    }
                }

                // Check wave completion
                int required = (currentWave == 1) ? 3 : (currentWave == 2) ? 5 : 10;
                if (enemiesSpawnedInWave >= required && enemies.empty()) {
                    if (currentWave < 3) {
                        currentWave++;
                        enemiesSpawnedInWave = 0;
                        waveTimer = 0.0f;
                    } else {
                        // All 3 waves defeated: Victory!
                        stateMachine.TriggerVictory();
                    }
                }
            }

            // 5. Update Entities & Projectiles
            for (auto& p : players) {
                if (p) p->Update(dt);
            }
            for (auto& e : enemies) {
                if (e) e->Update(dt, projectiles);
            }
            for (auto& proj : projectiles) {
                proj.Update(dt);
            }
            for (auto& col : collectibles) {
                col.Update(dt);
            }

            // 6. Collision & Combat Systems
            // Check Home Base Breach (Instant Loss)
            if (collisionSystem.CheckHomeBaseBreach(enemies, homeBaseMaxX)) {
                stateMachine.TriggerDefeat();
            }

            // Check Encounters (Stops and fights)
            collisionSystem.CheckPlayerEnemyEncounters(players, enemies);

            // Check Projectile Collisions (Applies dodges and hits)
            collisionSystem.CheckProjectileCollisions(projectiles, players, enemies);

            // Check Collectible Pickups
            collisionSystem.CheckPlayerCollectiblePickups(players, collectibles, totalCoins);

            // Run Combat Damage Exchange & Unit Cleanup
            combatSystem.Update(dt, players, enemies, collectibles);

            // Clean inactive projectiles & collectibles
            projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
                [](const Projectile& pr) { return !pr.IsActive(); }), projectiles.end());
            collectibles.erase(std::remove_if(collectibles.begin(), collectibles.end(),
                [](const Collectible& cl) { return !cl.IsActive(); }), collectibles.end());
        }
        else {
            // Update Menu UIs (Main Menu, Pause, Win/Lose, Sound Settings)
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
            // Gameplay Scene Rendering
            // 1. Grid Lanes (with hover & selection highlights)
            renderer.Draw(gridVAO, glm::vec2(0.0f), glm::vec2(1.0f));

            // Render Highlights over selected / hovered lanes
            const auto& lanes = grid.getLanes();
            for (int i = 0; i < static_cast<int>(lanes.size()); ++i) {
                float hl = selectedLanes[i] ? 0.70f : 0.0f;
                // Hover highlight
                if (mouseY >= lanes[i].minY && mouseY <= lanes[i].maxY &&
                    mouseX >= lanes[i].minX && mouseX <= lanes[i].maxX) {
                    hl = std::max(hl, 0.35f);
                }
                if (hl > 0.0f) {
                    renderer.DrawQuad(glm::vec2(lanes[i].minX, lanes[i].minY),
                                      glm::vec2(lanes[i].maxX - lanes[i].minX, lanes[i].maxY - lanes[i].minY),
                                      glm::vec4(1.0f, 0.95f, 0.30f, hl * 0.22f));
                }
            }

            // 2. Castle Environment Elements
            renderer.Draw(envVAO, glm::vec2(0.0f), glm::vec2(1.0f));

            // 3. Home Base (Castle Citadel Gate on the Left)
            // Outer stone tower
            renderer.DrawQuad(glm::vec2(0.0f, 60.0f), glm::vec2(98.0f, 480.0f),
                              glm::vec4(0.32f, 0.34f, 0.40f, 1.0f));
            // Battlements
            for (int b = 0; b < 6; ++b) {
                renderer.DrawQuad(glm::vec2(78.0f, 75.0f + b * 75.0f), glm::vec2(20.0f, 35.0f),
                                  glm::vec4(0.40f, 0.42f, 0.48f, 1.0f));
            }
            // Golden Citadel Crest & Princess Banner
            renderer.DrawQuad(glm::vec2(15.0f, 180.0f), glm::vec2(60.0f, 220.0f),
                              glm::vec4(0.60f, 0.15f, 0.65f, 1.0f)); // Royal Violet Banner
            renderer.DrawQuad(glm::vec2(25.0f, 220.0f), glm::vec2(40.0f, 40.0f),
                              glm::vec4(0.95f, 0.85f, 0.25f, 1.0f)); // Gold Crest

            // 4. Entities: Collectibles, Players, Enemies, Projectiles
            for (auto& col : collectibles) {
                col.Render(renderer);
            }
            for (auto& p : players) {
                if (p) p->Render(renderer);
            }
            for (auto& e : enemies) {
                if (e) e->Render(renderer);
            }
            for (auto& proj : projectiles) {
                proj.Render(renderer);
            }

            // 5. Gameplay HUD (Header bar: Coins, Wave, Controls)
            renderer.DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(screenWidth, 50.0f),
                              glm::vec4(0.08f, 0.10f, 0.15f, 0.90f));
            renderer.DrawQuad(glm::vec2(0.0f, 48.0f), glm::vec2(screenWidth, 2.0f),
                              glm::vec4(0.85f, 0.70f, 0.20f, 1.0f)); // Gold trim

            // Display Coins / Collectibles
            std::string coinStr = "GOLD: " + std::to_string(totalCoins);
            renderer.DrawString(coinStr, glm::vec2(20.0f, 16.0f), 1.25f, glm::vec4(1.0f, 0.88f, 0.25f, 1.0f));

            // Display Wave Info
            std::string waveStr = "WAVE: " + std::to_string(currentWave) + " / 3";
            renderer.DrawString(waveStr, glm::vec2(220.0f, 16.0f), 1.25f, glm::vec4(0.90f, 0.90f, 0.95f, 1.0f));

            // Controls Hint
            renderer.DrawString("[1] DEPLOY KNIGHT (10G)   [W/S] JUMP/DUCK   [ESC] PAUSE",
                                glm::vec2(400.0f, 18.0f), 0.82f, glm::vec4(0.70f, 0.75f, 0.85f, 1.0f));

            // 6. Overlays for Pause, Win, or Lose
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
