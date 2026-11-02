#pragma once

#include <glm/glm.hpp>
#include "../ui/MainMenuUI.h"
#include "../ui/PauseMenuUI.h"
#include "../ui/WinLoseMenuUI.h"
#include "../render/Renderer.h"

enum class GameState {
    MAIN_MENU = 0,
    CUTSCENE,
    GAMEPLAY,
    PAUSED,
    GAME_OVER_WIN,
    GAME_OVER_LOSE,
    SOUND_SETTINGS
};

class GameStateMachine {
public:
    GameStateMachine(float screenWidth, float screenHeight);

    void SetState(GameState newState);
    GameState GetState() const { return currentState; }

    bool IsInGameplay() const { return currentState == GameState::GAMEPLAY; }
    bool IsPaused() const { return currentState == GameState::PAUSED; }
    bool IsInCutscene() const { return currentState == GameState::CUTSCENE; }

    // Toggle pause with ESC
    void TogglePause();

    // Trigger Win or Lose
    void TriggerVictory();
    void TriggerDefeat();

    // Input & update for menus
    void Update(float dt, const glm::vec2& mousePos, bool mouseClicked);
    void RenderMenus(Renderer& renderer);

    // Callbacks consumed by main game loop
    bool WantsRestart() const { return requestRestart; }
    void ClearRestart() { requestRestart = false; }

    bool WantsExit() const { return requestExit; }
    void ClearExit() { requestExit = false; }

    MainMenuUI& GetMainMenu() { return mainMenu; }
    PauseMenuUI& GetPauseMenu() { return pauseMenu; }
    WinLoseMenuUI& GetWinLoseMenu() { return winLoseMenu; }

private:
    GameState currentState;
    GameState previousState;
    float screenWidth;
    float screenHeight;

    bool requestRestart;
    bool requestExit;

    MainMenuUI mainMenu;
    PauseMenuUI pauseMenu;
    WinLoseMenuUI winLoseMenu;
};

