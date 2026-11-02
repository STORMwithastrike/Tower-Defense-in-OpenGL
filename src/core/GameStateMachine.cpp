#include "GameStateMachine.h"

GameStateMachine::GameStateMachine(float screenWidth, float screenHeight)
    : currentState(GameState::MAIN_MENU),
      previousState(GameState::MAIN_MENU),
      screenWidth(screenWidth),
      screenHeight(screenHeight),
      requestRestart(false),
      requestExit(false),
      mainMenu(screenWidth, screenHeight),
      pauseMenu(screenWidth, screenHeight),
      winLoseMenu(screenWidth, screenHeight)
{
}

void GameStateMachine::SetState(GameState newState) {
    previousState = currentState;
    currentState = newState;
}

void GameStateMachine::TogglePause() {
    if (currentState == GameState::GAMEPLAY) {
        SetState(GameState::PAUSED);
    } else if (currentState == GameState::PAUSED) {
        SetState(GameState::GAMEPLAY);
    }
}

void GameStateMachine::TriggerVictory() {
    winLoseMenu.SetState(true);
    SetState(GameState::GAME_OVER_WIN);
}

void GameStateMachine::TriggerDefeat() {
    winLoseMenu.SetState(false);
    SetState(GameState::GAME_OVER_LOSE);
}

void GameStateMachine::Update(float dt, const glm::vec2& mousePos, bool mouseClicked) {
    switch (currentState) {
        case GameState::MAIN_MENU: {
            mainMenu.Update(dt, mousePos, mouseClicked);
            MainMenuAction action = mainMenu.ConsumeAction();
            if (action == MainMenuAction::START_GAME) {
                // Starts the animated cutscene, which then transitions into gameplay
                SetState(GameState::CUTSCENE);
            } else if (action == MainMenuAction::SOUND_SETTINGS) {
                SetState(GameState::SOUND_SETTINGS);
            } else if (action == MainMenuAction::EXIT_GAME) {
                requestExit = true;
            }
            break;
        }
        case GameState::PAUSED: {
            pauseMenu.Update(dt, mousePos, mouseClicked);
            PauseMenuAction action = pauseMenu.ConsumeAction();
            if (action == PauseMenuAction::CONTINUE_GAME) {
                SetState(GameState::GAMEPLAY);
            } else if (action == PauseMenuAction::RESTART_GAME) {
                requestRestart = true;
                SetState(GameState::GAMEPLAY);
            } else if (action == PauseMenuAction::RETURN_TO_MAIN_MENU) {
                SetState(GameState::MAIN_MENU);
            }
            break;
        }
        case GameState::GAME_OVER_WIN:
        case GameState::GAME_OVER_LOSE: {
            winLoseMenu.Update(dt, mousePos, mouseClicked);
            WinLoseMenuAction action = winLoseMenu.ConsumeAction();
            if (action == WinLoseMenuAction::RESTART_GAME) {
                requestRestart = true;
                SetState(GameState::GAMEPLAY);
            } else if (action == WinLoseMenuAction::RETURN_TO_MAIN_MENU) {
                SetState(GameState::MAIN_MENU);
            }
            break;
        }
        case GameState::SOUND_SETTINGS: {
            // Sound settings UI fallback (clicking anywhere returns to main menu)
            if (mouseClicked) {
                SetState(GameState::MAIN_MENU);
            }
            break;
        }
        default:
            break;
    }
}

void GameStateMachine::RenderMenus(Renderer& renderer) {
    switch (currentState) {
        case GameState::MAIN_MENU:
            mainMenu.Render(renderer);
            break;
        case GameState::PAUSED:
            pauseMenu.Render(renderer);
            break;
        case GameState::GAME_OVER_WIN:
        case GameState::GAME_OVER_LOSE:
            winLoseMenu.Render(renderer);
            break;
        case GameState::SOUND_SETTINGS: {
            // Sound settings modal
            renderer.DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(screenWidth, screenHeight),
                              glm::vec4(0.08f, 0.10f, 0.16f, 0.95f));
            glm::vec2 p((screenWidth - 420.0f) * 0.5f, 160.0f);
            renderer.DrawQuad(p - glm::vec2(2.0f, 2.0f), glm::vec2(424.0f, 264.0f),
                              glm::vec4(0.85f, 0.65f, 0.20f, 1.0f));
            renderer.DrawQuad(p, glm::vec2(420.0f, 260.0f),
                              glm::vec4(0.14f, 0.16f, 0.24f, 1.0f));
            renderer.DrawString("SOUND SETTINGS", p + glm::vec2(90.0f, 30.0f),
                                1.6f, glm::vec4(1.0f, 0.90f, 0.30f, 1.0f));
            renderer.DrawString("MASTER VOLUME: [ 80% ]", p + glm::vec2(60.0f, 90.0f),
                                1.1f, glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
            renderer.DrawString("BGM: ON   SFX: ON", p + glm::vec2(90.0f, 140.0f),
                                1.1f, glm::vec4(0.3f, 0.85f, 0.35f, 1.0f));
            renderer.DrawString("CLICK ANYWHERE TO RETURN", p + glm::vec2(60.0f, 210.0f),
                                1.0f, glm::vec4(0.6f, 0.7f, 0.85f, 1.0f));
            break;
        }
        default:
            break;
    }
}

