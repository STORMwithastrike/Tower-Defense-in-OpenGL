#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "../render/Renderer.h"

enum class MainMenuAction {
    NONE = 0,
    START_GAME,
    SOUND_SETTINGS,
    EXIT_GAME
};

class MainMenuUI {
public:
    MainMenuUI(float screenWidth, float screenHeight);

    void Update(float dt, const glm::vec2& mousePos, bool mouseClicked);
    void Render(Renderer& renderer);

    MainMenuAction ConsumeAction();

private:
    float screenWidth;
    float screenHeight;
    int selectedIndex;
    MainMenuAction triggeredAction;
    float animTimer;

    struct Button {
        std::string label;
        glm::vec2 pos;
        glm::vec2 size;
        MainMenuAction action;
        bool hovered;
    };

    std::vector<Button> buttons;
};

