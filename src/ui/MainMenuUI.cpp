#include "MainMenuUI.h"
#include <cmath>

MainMenuUI::MainMenuUI(float screenWidth, float screenHeight)
    : screenWidth(screenWidth),
      screenHeight(screenHeight),
      selectedIndex(-1),
      triggeredAction(MainMenuAction::NONE),
      animTimer(0.0f)
{
    float btnWidth = 260.0f;
    float btnHeight = 44.0f;
    float startX = (screenWidth - btnWidth) * 0.5f;
    float startY = 270.0f;
    float spacing = 58.0f;

    buttons = {
        { "START GAME",     glm::vec2(startX, startY),                 glm::vec2(btnWidth, btnHeight), MainMenuAction::START_GAME,     false },
        { "SOUND SETTINGS", glm::vec2(startX, startY + spacing),       glm::vec2(btnWidth, btnHeight), MainMenuAction::SOUND_SETTINGS, false },
        { "EXIT GAME",      glm::vec2(startX, startY + spacing * 2.0f), glm::vec2(btnWidth, btnHeight), MainMenuAction::EXIT_GAME,      false }
    };
}

void MainMenuUI::Update(float dt, const glm::vec2& mousePos, bool mouseClicked) {
    animTimer += dt;
    selectedIndex = -1;

    for (size_t i = 0; i < buttons.size(); ++i) {
        auto& btn = buttons[i];
        btn.hovered = (mousePos.x >= btn.pos.x && mousePos.x <= btn.pos.x + btn.size.x &&
                       mousePos.y >= btn.pos.y && mousePos.y <= btn.pos.y + btn.size.y);

        if (btn.hovered) {
            selectedIndex = static_cast<int>(i);
            if (mouseClicked) {
                triggeredAction = btn.action;
            }
        }
    }
}

MainMenuAction MainMenuUI::ConsumeAction() {
    MainMenuAction action = triggeredAction;
    triggeredAction = MainMenuAction::NONE;
    return action;
}

void MainMenuUI::Render(Renderer& renderer) {
    // 1. Dark Castle Night Sky & Backdrop
    renderer.DrawQuad(glm::vec2(0.0f, 0.0f), glm::vec2(screenWidth, screenHeight),
                      glm::vec4(0.08f, 0.10f, 0.16f, 1.0f));

    // Distant Castle Battlements silhouette on horizon
    for (int i = 0; i < 10; ++i) {
        float x = i * 85.0f;
        renderer.DrawQuad(glm::vec2(x, 180.0f), glm::vec2(45.0f, 60.0f), glm::vec4(0.12f, 0.14f, 0.22f, 1.0f));
        renderer.DrawQuad(glm::vec2(x + 45.0f, 200.0f), glm::vec2(40.0f, 40.0f), glm::vec4(0.10f, 0.12f, 0.18f, 1.0f));
    }

    // 2. Ornate Royal Heraldic Banner
    float bannerPulse = sinf(animTimer * 2.0f) * 3.0f;
    glm::vec2 titlePos((screenWidth - 460.0f) * 0.5f, 60.0f + bannerPulse);
    // Outer golden border
    renderer.DrawQuad(titlePos - glm::vec2(4.0f, 4.0f), glm::vec2(468.0f, 118.0f),
                      glm::vec4(0.85f, 0.65f, 0.15f, 1.0f));
    // Crimson velvet banner body
    renderer.DrawQuad(titlePos, glm::vec2(460.0f, 110.0f),
                      glm::vec4(0.55f, 0.08f, 0.12f, 1.0f));
    // Banner spear points (left & right)
    renderer.DrawTriangle(titlePos + glm::vec2(0.0f, 0.0f),
                          titlePos + glm::vec2(0.0f, 110.0f),
                          titlePos + glm::vec2(-25.0f, 55.0f),
                          glm::vec4(0.55f, 0.08f, 0.12f, 1.0f));
    renderer.DrawTriangle(titlePos + glm::vec2(460.0f, 0.0f),
                          titlePos + glm::vec2(460.0f, 110.0f),
                          titlePos + glm::vec2(485.0f, 55.0f),
                          glm::vec4(0.55f, 0.08f, 0.12f, 1.0f));

    // Title Text: CASTLE DEFENSE
    renderer.DrawString("CASTLE DEFENSE", titlePos + glm::vec2(60.0f, 25.0f),
                        2.1f, glm::vec4(1.0f, 0.90f, 0.25f, 1.0f));
    // Subtitle: PROTECT THE PRINCESS
    renderer.DrawString("PROTECT THE PRINCESS", titlePos + glm::vec2(95.0f, 75.0f),
                        1.2f, glm::vec4(0.95f, 0.95f, 0.95f, 1.0f));

    // 3. Render Interactive Menu Buttons
    for (const auto& btn : buttons) {
        glm::vec4 borderCol = btn.hovered ? glm::vec4(1.0f, 0.90f, 0.30f, 1.0f)
                                          : glm::vec4(0.40f, 0.40f, 0.45f, 1.0f);
        glm::vec4 bodyCol = btn.hovered ? glm::vec4(0.24f, 0.28f, 0.42f, 1.0f)
                                        : glm::vec4(0.16f, 0.18f, 0.26f, 1.0f);
        glm::vec4 textCol = btn.hovered ? glm::vec4(1.0f, 1.0f, 0.30f, 1.0f)
                                        : glm::vec4(0.85f, 0.85f, 0.90f, 1.0f);

        // Border & button body
        renderer.DrawQuad(btn.pos - glm::vec2(2.0f, 2.0f), btn.size + glm::vec2(4.0f, 4.0f), borderCol);
        renderer.DrawQuad(btn.pos, btn.size, bodyCol);

        // Center button label text
        float textX = btn.pos.x + (btn.size.x - (btn.label.length() * 11.0f * 1.25f)) * 0.5f;
        float textY = btn.pos.y + 14.0f;
        renderer.DrawString(btn.label, glm::vec2(textX, textY), 1.25f, textCol);
    }

    // Footer instructions
    renderer.DrawString("CLICK TO SELECT - ESC TO PAUSE IN GAME",
                        glm::vec2((screenWidth - 340.0f) * 0.5f, screenHeight - 40.0f),
                        0.95f, glm::vec4(0.6f, 0.65f, 0.75f, 1.0f));
}

