#include "PauseMenuUI.h"

PauseMenuUI::PauseMenuUI(float screenWidth, float screenHeight)
    : screenWidth(screenWidth),
      screenHeight(screenHeight),
      triggeredAction(PauseMenuAction::NONE)
{
    float btnWidth = 260.0f;
    float btnHeight = 44.0f;
    float startX = (screenWidth - btnWidth) * 0.5f;
    float startY = 230.0f;
    float spacing = 58.0f;

    buttons = {
        { "CONTINUE",    glm::vec2(startX, startY),                 glm::vec2(btnWidth, btnHeight), PauseMenuAction::CONTINUE_GAME,        false },
        { "RESTART",     glm::vec2(startX, startY + spacing),       glm::vec2(btnWidth, btnHeight), PauseMenuAction::RESTART_GAME,         false },
        { "MAIN MENU",   glm::vec2(startX, startY + spacing * 2.0f), glm::vec2(btnWidth, btnHeight), PauseMenuAction::RETURN_TO_MAIN_MENU, false }
    };
}

void PauseMenuUI::Update(float dt, const glm::vec2& mousePos, bool mouseClicked) {
    (void)dt;
    for (auto& btn : buttons) {
        btn.hovered = (mousePos.x >= btn.pos.x && mousePos.x <= btn.pos.x + btn.size.x &&
                       mousePos.y >= btn.pos.y && mousePos.y <= btn.pos.y + btn.size.y);

        if (btn.hovered && mouseClicked) {
            triggeredAction = btn.action;
        }
    }
}

PauseMenuAction PauseMenuUI::ConsumeAction() {
    PauseMenuAction action = triggeredAction;
    triggeredAction = PauseMenuAction::NONE;
    return action;
}

void PauseMenuUI::Render(Renderer& renderer) {
    // 1. Dark semi-transparent overlay to dim gameplay background
    renderer.DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(screenWidth, screenHeight),
                      glm::vec4(0.04f, 0.05f, 0.08f, 0.78f));

    // 2. Pause Modal Panel
    glm::vec2 panelPos((screenWidth - 380.0f) * 0.5f, 130.0f);
    glm::vec2 panelSize(380.0f, 320.0f);
    renderer.DrawQuad(panelPos - glm::vec2(3.0f, 3.0f), panelSize + glm::vec2(6.0f, 6.0f),
                      glm::vec4(0.70f, 0.55f, 0.20f, 1.0f)); // Gold trim
    renderer.DrawQuad(panelPos, panelSize,
                      glm::vec4(0.12f, 0.14f, 0.20f, 0.95f)); // Stone slate body

    // Header Title: GAME PAUSED
    renderer.DrawString("GAME PAUSED", panelPos + glm::vec2(85.0f, 30.0f),
                        1.7f, glm::vec4(1.0f, 0.90f, 0.30f, 1.0f));

    // 3. Render Buttons
    for (const auto& btn : buttons) {
        glm::vec4 borderCol = btn.hovered ? glm::vec4(1.0f, 0.90f, 0.30f, 1.0f)
                                          : glm::vec4(0.40f, 0.40f, 0.45f, 1.0f);
        glm::vec4 bodyCol = btn.hovered ? glm::vec4(0.24f, 0.28f, 0.42f, 1.0f)
                                        : glm::vec4(0.18f, 0.20f, 0.28f, 1.0f);
        glm::vec4 textCol = btn.hovered ? glm::vec4(1.0f, 1.0f, 0.30f, 1.0f)
                                        : glm::vec4(0.90f, 0.90f, 0.95f, 1.0f);

        renderer.DrawQuad(btn.pos - glm::vec2(2.0f, 2.0f), btn.size + glm::vec2(4.0f, 4.0f), borderCol);
        renderer.DrawQuad(btn.pos, btn.size, bodyCol);

        float textX = btn.pos.x + (btn.size.x - (btn.label.length() * 11.0f * 1.25f)) * 0.5f;
        float textY = btn.pos.y + 14.0f;
        renderer.DrawString(btn.label, glm::vec2(textX, textY), 1.25f, textCol);
    }
}

