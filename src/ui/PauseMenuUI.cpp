#include "PauseMenuUI.h"

PauseMenuUI::PauseMenuUI(float screenWidth, float screenHeight)
    : screenWidth(screenWidth),
      screenHeight(screenHeight),
      triggeredAction(PauseMenuAction::NONE)
{
    float btnWidth = 280.0f;
    float btnHeight = 44.0f;
    float startX = (screenWidth - btnWidth) * 0.5f;
    float startY = 220.0f;
    float spacing = 60.0f;

    buttons = {
        { "RESUME",              glm::vec2(startX, startY),                  glm::vec2(btnWidth, btnHeight), PauseMenuAction::RESUME_GAME,         false },
        { "RESTART",             glm::vec2(startX, startY + spacing),        glm::vec2(btnWidth, btnHeight), PauseMenuAction::RESTART_GAME,        false },
        { "QUIT TO MAIN MENU",   glm::vec2(startX, startY + spacing * 2.0f),  glm::vec2(btnWidth, btnHeight), PauseMenuAction::RETURN_TO_MAIN_MENU, false }
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
    // 1. Black background with exactly 70% opacity
    renderer.DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(screenWidth, screenHeight),
                      glm::vec4(0.0f, 0.0f, 0.0f, 0.70f));

    // 2. Title: GAME PAUSED
    float titleScale = 1.9f;
    std::string titleText = "GAME PAUSED";
    float titleX = (screenWidth - (titleText.length() * 11.0f * titleScale)) * 0.5f;
    renderer.DrawString(titleText, glm::vec2(titleX, 130.0f), titleScale,
                        glm::vec4(1.0f, 0.90f, 0.25f, 1.0f));

    // Subtitle hint
    std::string hintText = "PRESS ESC TO RESUME";
    float hintScale = 0.90f;
    float hintX = (screenWidth - (hintText.length() * 11.0f * hintScale)) * 0.5f;
    renderer.DrawString(hintText, glm::vec2(hintX, 175.0f), hintScale,
                        glm::vec4(0.70f, 0.75f, 0.85f, 1.0f));

    // 3. Render Buttons (stylized identically to MainMenuUI)
    for (const auto& btn : buttons) {
        glm::vec4 borderCol = btn.hovered ? glm::vec4(1.0f, 0.90f, 0.30f, 1.0f)
                                          : glm::vec4(0.40f, 0.40f, 0.45f, 1.0f);
        glm::vec4 bodyCol = btn.hovered ? glm::vec4(0.24f, 0.28f, 0.42f, 1.0f)
                                        : glm::vec4(0.16f, 0.18f, 0.26f, 1.0f);
        glm::vec4 textCol = btn.hovered ? glm::vec4(1.0f, 1.0f, 0.30f, 1.0f)
                                        : glm::vec4(0.85f, 0.85f, 0.90f, 1.0f);

        // Border & button body (same styling as main menu)
        renderer.DrawQuad(btn.pos - glm::vec2(2.0f, 2.0f), btn.size + glm::vec2(4.0f, 4.0f), borderCol);
        renderer.DrawQuad(btn.pos, btn.size, bodyCol);

        // Center button label text
        float textScale = (btn.label.length() > 10) ? 1.10f : 1.25f;
        float textX = btn.pos.x + (btn.size.x - (btn.label.length() * 11.0f * textScale)) * 0.5f;
        float textY = btn.pos.y + (btn.size.y - (14.0f * textScale)) * 0.5f + 2.0f;
        renderer.DrawString(btn.label, glm::vec2(textX, textY), textScale, textCol);
    }
}

