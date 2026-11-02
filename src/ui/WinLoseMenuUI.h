#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "../render/Renderer.h"

enum class WinLoseMenuAction {
    NONE = 0,
    RESTART_GAME,
    RETURN_TO_MAIN_MENU
};

class WinLoseMenuUI {
public:
    WinLoseMenuUI(float screenWidth, float screenHeight);

    void SetState(bool won);
    void Update(float dt, const glm::vec2& mousePos, bool mouseClicked);
    void Render(Renderer& renderer);

    WinLoseMenuAction ConsumeAction();

private:
    float screenWidth;
    float screenHeight;
    bool isVictory;
    WinLoseMenuAction triggeredAction;
    float animTimer;

    struct Button {
        std::string label;
        glm::vec2 pos;
        glm::vec2 size;
        WinLoseMenuAction action;
        bool hovered;
    };

    std::vector<Button> buttons;
};

