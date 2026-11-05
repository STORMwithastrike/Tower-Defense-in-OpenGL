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
#include "game/Enemy.h"
#include "game/EasyEnemy.h"
#include "game/EnemyProjectile.h"
#include "game/HardEnemy.h"
#include "game/MediumEnemy.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <vector>

int main() {
  const float screenWidth = 800.0f;
  const float screenHeight = 600.0f;

  Window gameWindow((int)screenWidth, (int)screenHeight);
  if (!gameWindow.initialize("Castle Defense - Protect the Princess")) {
    std::cerr << "Failed to initialize Game Window!" << std::endl;
    return -1;
  }

  GLFWwindow *rawWindow = gameWindow.getGLFWWindow();
  glfwSetKeyCallback(rawWindow, InputManager::keyCallback);
  glfwSetMouseButtonCallback(rawWindow, InputManager::mouseButtonCallback);
  glfwSetCursorPosCallback(rawWindow, InputManager::cursorPosCallback);

  // shader, camera and renderer setup
  Shader mainShader(ASSETS_DIR "shaders/main.vert",
                    ASSETS_DIR "shaders/main.frag");
  Camera camera(screenWidth, screenHeight);
  Renderer renderer(&mainShader, screenWidth, screenHeight);
  renderer.SetCamera(&camera);

  // Enable alpha blending for UI transparencies and 70% opacity pause background
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // grid and background
  Grid gameGrid;
  gameGrid.init();
  VertexArray gridVAO(gameGrid.getVertices(), gameGrid.getIndices());

  Environment environment;
  environment.init();
  VertexArray envVAO(environment.getVertices(), environment.getIndices());

  HomeBase homeBase(0.0f, 60.0f, 98.0f, 480.0f, 15);
  LaneSelector laneSelector;

  GameStateMachine stateMachine(screenWidth, screenHeight);
  CutsceneManager cutscene(screenWidth, screenHeight);

  WaveManager waveManager;
  CollisionSystem collisionSystem;
  CombatSystem combatSystem;

  // prototype meshes for players
  MeleePlayer protoMelee(0);
  protoMelee.init(0.0f, 0.0f);
  VertexArray meleeVAO(protoMelee.getVertices(), protoMelee.getIndices());

  RangedPlayer protoRanged(0);
  protoRanged.init(0.0f, 0.0f);
  VertexArray rangedVAO(protoRanged.getVertices(), protoRanged.getIndices());

  DefensePlayer protoDefense(0);
  protoDefense.init(0.0f, 0.0f);
  VertexArray defenseVAO(protoDefense.getVertices(), protoDefense.getIndices());

  std::vector<std::unique_ptr<Player>> spawnedPlayers;

  // arrows and projectiles
  std::vector<Projectile> playerArrows;
  Projectile arrowProto;
  arrowProto.init(0.0f, 0.0f);
  VertexArray projectileVAO(arrowProto.getVertices(), arrowProto.getIndices());

  std::vector<std::unique_ptr<Enemy>> enemies;
  std::vector<EnemyProjectile> enemyProjectiles;
  std::vector<Collectible> collectibles;

  Economy economy(60);

  std::vector<PlayerCardUI> unitCards;
  // Bottom center card placement: screen is 800 wide, 3 cards centered between
  // X=255 and X=545, Y=548
  unitCards.emplace_back(CardUnitType::Melee, 10, glm::vec2(255.0f, 548.0f),
                         glm::vec2(86.0f, 44.0f));
  unitCards.emplace_back(CardUnitType::Ranged, 15, glm::vec2(357.0f, 548.0f),
                         glm::vec2(86.0f, 44.0f));
  unitCards.emplace_back(CardUnitType::Defense, 20, glm::vec2(459.0f, 548.0f),
                         glm::vec2(86.0f, 44.0f));

  auto restartGame = [&]() {
    economy.reset();
    for (auto &card : unitCards) {
      card.setSelected(false);
    }

    homeBase.reset();
    waveManager.startWave(1);
    enemies.clear();
    enemyProjectiles.clear();
    playerArrows.clear();
    collectibles.clear();
    spawnedPlayers.clear();
    combatSystem.ResetDefeatedEnemyCount();
  };

  restartGame();

  bool prevMouseLeft = false;
  bool prevEscKey = false;
  bool prevCKey = false;

  double lastTime = glfwGetTime();

  while (!gameWindow.shouldClose() && !stateMachine.WantsExit()) {
    double currentTime = glfwGetTime();
    float dt = (float)(currentTime - lastTime);
    if (dt > 0.1f)
      dt = 0.1f;
    lastTime = currentTime;

    if (stateMachine.WantsRestart()) {
      stateMachine.ClearRestart();
      restartGame();
    }

    double mouseX = InputManager::getMouseX();
    double mouseY = InputManager::getMouseY();
    glm::vec2 mousePos((float)mouseX, (float)mouseY);

    bool mouseLeftDown =
        InputManager::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
    bool mouseClicked = (mouseLeftDown && !prevMouseLeft);
    prevMouseLeft = mouseLeftDown;

    bool escDown = InputManager::isKeyPressed(GLFW_KEY_ESCAPE);
    bool escPressed = (escDown && !prevEscKey);
    prevEscKey = escDown;

    bool spaceDown = InputManager::isKeyPressed(GLFW_KEY_SPACE);

    // debug add coins
    bool cDown = InputManager::isKeyPressed(GLFW_KEY_C);
    if (cDown && !prevCKey) {
      economy.addCoins(20);
    }
    prevCKey = cDown;

    if (escPressed && (stateMachine.GetState() == GameState::GAMEPLAY ||
                       stateMachine.GetState() == GameState::PAUSED)) {
      stateMachine.TogglePause();
    }

    camera.Update(dt);

    if (stateMachine.GetState() == GameState::CUTSCENE) {
      cutscene.Update(dt, &camera, spaceDown);
      if (cutscene.IsFinished()) {
        stateMachine.SetState(GameState::GAMEPLAY);
      }
    } else if (stateMachine.GetState() == GameState::GAMEPLAY) {
      // update cards
      for (size_t i = 0; i < unitCards.size(); ++i) {
        unitCards[i].update(economy, mouseX, mouseY);

        if (unitCards[i].handleMouseClick(mouseX, mouseY, mouseLeftDown)) {
          if (unitCards[i].isSelected()) {
            for (size_t j = 0; j < unitCards.size(); ++j) {
              if (i != j) {
                unitCards[j].setSelected(false);
              }
            }
          }
        }
      }

      laneSelector.update(gameGrid, mouseX, mouseY, mouseClicked);

      // buy and place units
      PlayerCardUI *selectedCard = nullptr;
      for (auto &card : unitCards) {
        if (card.isSelected()) {
          selectedCard = &card;
          break;
        }
      }

      if (selectedCard && mouseClicked && mouseY < 544.0f &&
          laneSelector.getHoveredLaneId() != -1) {
        int targetLane = laneSelector.getHoveredLaneId();
        float targetY = gameGrid.getLaneCenterY(targetLane);

        if (economy.spendCoins(selectedCard->getCost())) {
          if (selectedCard->getUnitType() == CardUnitType::Melee) {
            auto newUnit = std::make_unique<MeleePlayer>(targetLane);
            newUnit->init(120.0f, targetY);
            newUnit->deploy(120.0f, targetY, targetLane);
            spawnedPlayers.push_back(std::move(newUnit));
          } else if (selectedCard->getUnitType() == CardUnitType::Ranged) {
            auto newUnit = std::make_unique<RangedPlayer>(targetLane);
            newUnit->init(120.0f, targetY);
            newUnit->deploy(120.0f, targetY, targetLane);
            spawnedPlayers.push_back(std::move(newUnit));
          } else if (selectedCard->getUnitType() == CardUnitType::Defense) {
            float placeX = (float)mouseX;
            if (placeX < 120.0f)
              placeX = 120.0f;
            if (placeX > 740.0f)
              placeX = 740.0f;

            auto newUnit = std::make_unique<DefensePlayer>(targetLane);
            newUnit->init(placeX, targetY);
            newUnit->deploy(placeX, targetY, targetLane);
            spawnedPlayers.push_back(std::move(newUnit));
          }
        }

        selectedCard->setSelected(false);
      }

      std::vector<Player *> activePlayers;
      activePlayers.reserve(spawnedPlayers.size());
      for (auto &p : spawnedPlayers) {
        if (p && p->isAlive()) {
          activePlayers.push_back(p.get());
        }
      }

      // jump / duck input
      std::vector<int> selectedLanes;
      if (laneSelector.getSelectedLaneId() != -1) {
        selectedLanes.push_back(laneSelector.getSelectedLaneId());
      }
      ActionManager::handleInput(activePlayers, selectedLanes);

      for (auto &p : spawnedPlayers) {
        if (p && p->isAlive()) {
          // Auto attack animation for melee while in combat
          if (p->getIsFighting() && p->canAttack() &&
              dynamic_cast<MeleePlayer *>(p.get())) {
            p->attack();
            p->resetAttackCooldown();
          }
          p->update(dt);
        }
      }

      // attack input (spacebar)
      if (spaceDown) {
        for (auto &p : spawnedPlayers) {
          if (p && p->isAlive()) {
            if (dynamic_cast<RangedPlayer *>(p.get())) {
              if (p->canAttack()) {
                Projectile arrow;
                arrow.init(p->getPosition().x + 35.0f,
                           p->getPosition().y + 5.0f);
                playerArrows.push_back(arrow);
                p->resetAttackCooldown();
                p->attack();
              }
            } else if (dynamic_cast<MeleePlayer *>(p.get())) {
              if (p->canAttack()) {
                p->attack();
                p->resetAttackCooldown();
              }
            }
          }
        }
      }

      // wave spawns
      EnemyType spawnType;
      int spawnLane = 0;
      if (waveManager.shouldSpawnEnemy(dt, gameGrid.getLaneCount(), spawnType,
                                       spawnLane)) {
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

      if (waveManager.isSpawningFinished() && enemies.empty()) {
        waveManager.onWaveEnemiesCleared();
        if (waveManager.isGameWon()) {
          stateMachine.TriggerVictory();
        }
      }

      for (auto &e : enemies) {
        if (e)
          e->Update(dt, enemyProjectiles);
      }
      for (auto &ep : enemyProjectiles) {
        ep.Update(dt);
      }
      for (auto &arrow : playerArrows) {
        if (arrow.isActive())
          arrow.update();
      }
      for (auto &col : collectibles) {
        col.Update(dt);
      }

      if (collisionSystem.CheckHomeBaseBreach(enemies, homeBase)) {
        stateMachine.TriggerDefeat();
      }

      collisionSystem.CheckPlayerEnemyEncounters(activePlayers, enemies);
      collisionSystem.CheckPlayerArrows(playerArrows, enemies);

      int preHealth = homeBase.getHealth();
      collisionSystem.CheckEnemyProjectiles(enemyProjectiles, activePlayers,
                                            homeBase);
      if (homeBase.getHealth() < preHealth) {
        camera.TriggerShake(0.2f, 3.5f);
      }

      if (homeBase.hasLostGame()) {
        stateMachine.TriggerDefeat();
      }

      // pickup gold
      int collectedGold = economy.getCoins();
      collisionSystem.CheckPlayerCollectiblePickups(activePlayers, collectibles,
                                                    collectedGold);

      if (mouseClicked) {
        for (auto &col : collectibles) {
          if (!col.IsActive())
            continue;
          BoundingBox cBox = col.GetBounds();
          if (mousePos.x >= cBox.x && mousePos.x <= cBox.x + cBox.width &&
              mousePos.y >= cBox.y && mousePos.y <= cBox.y + cBox.height) {
            col.Collect();
            collectedGold += col.GetValue();
          }
        }
      }
      economy.setCoins(collectedGold);

      combatSystem.Update(dt, activePlayers, enemies, collectibles);

      // cleanup inactive entities
      for (auto it = spawnedPlayers.begin(); it != spawnedPlayers.end();) {
        if (!(*it) || !(*it)->isAlive()) {
          it = spawnedPlayers.erase(it);
        } else {
          ++it;
        }
      }

      for (auto it = enemyProjectiles.begin(); it != enemyProjectiles.end();) {
        if (!it->IsActive()) {
          it = enemyProjectiles.erase(it);
        } else {
          ++it;
        }
      }

      for (auto it = playerArrows.begin(); it != playerArrows.end();) {
        if (!it->isActive()) {
          it = playerArrows.erase(it);
        } else {
          ++it;
        }
      }

      for (auto it = collectibles.begin(); it != collectibles.end();) {
        if (!it->IsActive()) {
          it = collectibles.erase(it);
        } else {
          ++it;
        }
      }
    } else {
      stateMachine.Update(dt, mousePos, mouseClicked);
      if (stateMachine.GetState() == GameState::CUTSCENE) {
        cutscene.Start();
      }
    }

    // render scene
    glClearColor(0.10f, 0.12f, 0.18f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Helper lambda to draw the gameplay field (lanes, environment, castle
    // base, princess) Used by BOTH gameplay and cutscene so backgrounds are
    // identical
    auto renderGameplayField = [&]() {
      renderer.Draw(gridVAO, glm::vec2(0.0f), glm::vec2(1.0f));
      renderer.Draw(envVAO, glm::vec2(0.0f), glm::vec2(1.0f));

      // Castle tower & base
      glm::vec4 towerColor = homeBase.hasLostGame()
                                 ? glm::vec4(0.65f, 0.15f, 0.15f, 1.0f)
                                 : glm::vec4(0.32f, 0.34f, 0.40f, 1.0f);
      renderer.DrawQuad(homeBase.getPosition(), homeBase.getSize(), towerColor);

      for (int b = 0; b < 6; ++b) {
        renderer.DrawQuad(glm::vec2(78.0f, 75.0f + b * 75.0f),
                          glm::vec2(20.0f, 35.0f),
                          glm::vec4(0.40f, 0.42f, 0.48f, 1.0f));
      }
      renderer.DrawQuad(glm::vec2(25.0f, 470.0f), glm::vec2(50.0f, 70.0f),
                        glm::vec4(0.18f, 0.18f, 0.22f, 1.0f));

      // Balcony
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

      // Princess on balcony
      glm::vec2 princessPos = balconyPos + glm::vec2(26.0f, 16.0f);
      float pBob = sinf((float)currentTime * 3.0f) * 1.5f;
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
    };

    if (stateMachine.GetState() == GameState::CUTSCENE) {
      // Cutscene rendered directly on top of the gameplay fields
      renderGameplayField();
      cutscene.Render(renderer);
    } else if (stateMachine.GetState() == GameState::MAIN_MENU ||
               stateMachine.GetState() == GameState::SOUND_SETTINGS) {
      stateMachine.RenderMenus(renderer);
    } else {
      renderGameplayField();

      // lane selection highlight
      const auto &lanes = gameGrid.getLanes();
      for (const auto &lane : lanes) {
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
        } else if (lane.id == laneSelector.getHoveredLaneId()) {
          renderer.DrawQuad(pos, size, glm::vec4(0.16f, 0.18f, 0.22f, 0.52f));
          renderer.DrawQuad(glm::vec2(lane.minX, lane.minY),
                            glm::vec2(size.x, 1.8f),
                            glm::vec4(0.35f, 0.38f, 0.44f, 0.75f));
          renderer.DrawQuad(glm::vec2(lane.minX, lane.maxY - 1.8f),
                            glm::vec2(size.x, 1.8f),
                            glm::vec4(0.35f, 0.38f, 0.44f, 0.75f));
        }
      }

      // Castle HP display using HealthUI
      renderer.DrawString("CASTLE HP", glm::vec2(8.0f, 68.0f), 0.82f,
                          glm::vec4(1.0f, 0.90f, 0.30f, 1.0f));
      HealthUI::draw(renderer, glm::vec2(8.0f, 82.0f), 82.0f,
                     homeBase.getHealth(), homeBase.getMaxHealth(), 0.0f, 82.0f,
                     9.0f);
      std::string castleHpNum = std::to_string(homeBase.getHealth()) + " / " +
                                std::to_string(homeBase.getMaxHealth());
      renderer.DrawString(castleHpNum, glm::vec2(26.0f, 94.0f), 0.75f,
                          glm::vec4(0.95f, 0.95f, 0.95f, 1.0f));

      // collectibles
      for (auto &col : collectibles) {
        col.Render(renderer);
      }

      // draw players and health bars via HealthUI
      for (const auto &p : spawnedPlayers) {
        if (!p || !p->isAlive())
          continue;

        if (dynamic_cast<MeleePlayer *>(p.get())) {
          renderer.Draw(meleeVAO, p->getRenderPosition(), glm::vec2(1.0f, 1.0f),
                        0.0f, glm::vec4(1.0f), p->getRotation());
        } else if (dynamic_cast<RangedPlayer *>(p.get())) {
          renderer.Draw(rangedVAO, p->getRenderPosition(),
                        glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f),
                        p->getRotation());
        } else if (dynamic_cast<DefensePlayer *>(p.get())) {
          renderer.Draw(defenseVAO, p->getRenderPosition(),
                        glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));
        }

        glm::vec2 rPos = p->getRenderPosition();
        HealthUI::draw(renderer, rPos, 20.0f, p->getHealth(), p->getMaxHealth(),
                       -25.0f, 32.0f, 5.0f);
      }

      for (auto &arrow : playerArrows) {
        if (arrow.isActive()) {
          renderer.Draw(projectileVAO, arrow.getPosition(),
                        glm::vec2(1.0f, 1.0f), 0.0f, glm::vec4(1.0f));
        }
      }

      for (auto &e : enemies) {
        if (e)
          e->Render(renderer);
      }
      for (auto &ep : enemyProjectiles) {
        ep.Render(renderer);
      }

      // Top HUD Bar (Gold, Waves, Controls hint)
      renderer.DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(screenWidth, 46.0f),
                        glm::vec4(0.08f, 0.10f, 0.15f, 0.90f));
      renderer.DrawQuad(glm::vec2(0.0f, 44.0f), glm::vec2(screenWidth, 2.0f),
                        glm::vec4(0.85f, 0.70f, 0.20f, 1.0f));

      std::string goldStr = "GOLD: " + std::to_string(economy.getCoins());
      renderer.DrawString(goldStr, glm::vec2(20.0f, 14.0f), 1.15f,
                          glm::vec4(1.0f, 0.88f, 0.25f, 1.0f));

      std::string waveStr =
          "WAVE: " + std::to_string(waveManager.getCurrentWave()) + "/3";
      renderer.DrawString(waveStr, glm::vec2(160.0f, 14.0f), 1.15f,
                          glm::vec4(0.90f, 0.90f, 0.95f, 1.0f));

      renderer.DrawString("[SPACE] SHOOT   [W/S] JUMP/DUCK",
                          glm::vec2(470.0f, 16.0f), 0.72f,
                          glm::vec4(0.70f, 0.75f, 0.85f, 1.0f));

      // Player Cards placed at bottom center
      renderer.DrawQuad(glm::vec2(245.0f, 542.0f), glm::vec2(310.0f, 56.0f),
                        glm::vec4(0.08f, 0.10f, 0.15f, 0.90f));
      renderer.DrawQuad(glm::vec2(245.0f, 542.0f), glm::vec2(310.0f, 2.0f),
                        glm::vec4(0.85f, 0.70f, 0.20f, 0.85f));

      for (const auto &card : unitCards) {
        card.render(renderer);
      }

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