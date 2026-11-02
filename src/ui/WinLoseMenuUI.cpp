#include "WinLoseMenuUI.h"
#include <cmath>

WinLoseMenuUI::WinLoseMenuUI(float screenWidth, float screenHeight)
    : screenWidth(screenWidth),
      screenHeight(screenHeight),
      isVictory(false),
      triggeredAction(WinLoseMenuAction::NONE),
      animTimer(0.0f)
{
    float btnWidth = 240.0f;
    float btnHeight = 44.0f;
    float startX = (screenWidth - btnWidth) * 0.5f;
    float startY = 310.0f;
    float spacing = 58.0f;

    buttons = {
        { "RESTART",   glm::vec2(startX, startY),           glm::vec2(btnWidth, btnHeight), WinLoseMenuAction::RESTART_GAME,         false },
        { "MAIN MENU", glm::vec2(startX, startY + spacing), glm::vec2(btnWidth, btnHeight), WinLoseMenuAction::RETURN_TO_MAIN_MENU, false }
    };
}

void WinLoseMenuUI::SetState(bool won) {
    isVictory = won;
    triggeredAction = WinLoseMenuAction::NONE;
}

void WinLoseMenuUI::Update(float dt, const glm::vec2& mousePos, bool mouseClicked) {
    animTimer += dt;
    for (auto& btn : buttons) {
        btn.hovered = (mousePos.x >= btn.pos.x && mousePos.x <= btn.pos.x + btn.size.x &&
                       mousePos.y >= btn.pos.y && mousePos.y <= btn.pos.y + btn.size.y);

        if (btn.hovered && mouseClicked) {
            triggeredAction = btn.action;
        }
    }
}

WinLoseMenuAction WinLoseMenuUI::ConsumeAction() {
    WinLoseMenuAction action = triggeredAction;
    triggeredAction = WinLoseMenuAction::NONE;
    return action;
}

void WinLoseMenuUI::Render(Renderer& renderer) {
    // 1. Dim background
    renderer.DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(screenWidth, screenHeight),
                      glm::vec4(0.03f, 0.04f, 0.06f, 0.85f));

    // 2. Banner Box
    glm::vec2 bannerPos((screenWidth - 500.0f) * 0.5f, 110.0f);
    glm::vec2 bannerSize(500.0f, 160.0f);

    glm::vec4 trimColor = isVictory ? glm::vec4(0.95f, 0.80f, 0.20f, 1.0f)  // Gold
                                    : glm::vec4(0.75f, 0.15f, 0.15f, 1.0f); // Blood Red
    glm::vec4 bodyColor = isVictory ? glm::vec4(0.10f, 0.25f, 0.45f, 0.95f) // Royal Blue
                                    : glm::vec4(0.20f, 0.08f, 0.08f, 0.95f); // Dark Charcoal Red

    renderer.DrawQuad(bannerPos - glm::vec2(3.0f, 3.0f), bannerSize + glm::vec2(6.0f, 6.0f), trimColor);
    renderer.DrawQuad(bannerPos, bannerSize, bodyColor);

    if (isVictory) {
        renderer.DrawString("VICTORY!", bannerPos + glm::vec2(165.0f, 32.0f),
                            2.4f, glm::vec4(1.0f, 0.90f, 0.20f, 1.0f));
        renderer.DrawString("THE PRINCESS AND CASTLE ARE SAVED!", bannerPos + glm::vec2(45.0f, 95.0f),
                            1.15f, glm::vec4(0.90f, 0.95f, 1.0f, 1.0f));
    } else {
        renderer.DrawString("DEFEAT!", bannerPos + glm::vec2(180.0f, 32.0f),
                            2.4f, glm::vec4(0.95f, 0.20f, 0.20f, 1.0f));
        renderer.DrawString("THE CASTLE HAS FALLEN TO THE INVADERS!", bannerPos + glm::vec2(35.0f, 95.0f),
                            1.15f, glm::vec4(0.90f, 0.80f, 0.80f, 1.0f));
    }

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

