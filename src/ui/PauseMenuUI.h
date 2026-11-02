#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "../render/Renderer.h"

enum class PauseMenuAction {
    NONE = 0,
    CONTINUE_GAME,
    RESTART_GAME,
    RETURN_TO_MAIN_MENU
};

class PauseMenuUI {
public:
    PauseMenuUI(float screenWidth, float screenHeight);

    void Update(float dt, const glm::vec2& mousePos, bool mouseClicked);
    void Render(Renderer& renderer);

    PauseMenuAction ConsumeAction();

private:
    float screenWidth;
    float screenHeight;
    PauseMenuAction triggeredAction;

    struct Button {
        std::string label;
        glm::vec2 pos;
        glm::vec2 size;
        PauseMenuAction action;
        bool hovered;
    };

    std::vector<Button> buttons;
};

