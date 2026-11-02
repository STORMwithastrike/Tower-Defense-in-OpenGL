#include "SubtitleUI.h"
#include <cmath>

SubtitleUI::SubtitleUI(float screenWidth, float screenHeight)
    : screenWidth(screenWidth),
      screenHeight(screenHeight),
      visible(false),
      animTimer(0.0f)
{
}

void SubtitleUI::SetSubtitle(const std::string& speaker, const std::string& line1, const std::string& line2) {
    currentSpeaker = speaker;
    textLine1 = line1;
    textLine2 = line2;
    visible = true;
    animTimer = 0.0f;
}

void SubtitleUI::Clear() {
    visible = false;
    currentSpeaker.clear();
    textLine1.clear();
    textLine2.clear();
}

void SubtitleUI::Update(float dt) {
    if (visible) {
        animTimer += dt;
    }
}

void SubtitleUI::Render(Renderer& renderer) {
    if (!visible) return;

    // Dimensions for dialog box at bottom of screen
    float boxWidth = 720.0f;
    float boxHeight = 95.0f;
    glm::vec2 boxPos((screenWidth - boxWidth) * 0.5f, screenHeight - 115.0f);

    // 1. Ornate Border & Background
    renderer.DrawQuad(boxPos - glm::vec2(2.5f, 2.5f), glm::vec2(boxWidth + 5.0f, boxHeight + 5.0f),
                      glm::vec4(0.85f, 0.70f, 0.25f, 1.0f)); // Gold trim
    renderer.DrawQuad(boxPos, glm::vec2(boxWidth, boxHeight),
                      glm::vec4(0.10f, 0.12f, 0.18f, 0.92f)); // Dark slate body

    // 2. Speaker Badge / Nametag
    if (!currentSpeaker.empty()) {
        glm::vec2 badgePos = boxPos + glm::vec2(20.0f, -14.0f);
        float badgeWidth = (currentSpeaker.length() * 11.0f * 1.0f) + 20.0f;
        renderer.DrawQuad(badgePos - glm::vec2(1.5f, 1.5f), glm::vec2(badgeWidth + 3.0f, 22.0f),
                          glm::vec4(0.85f, 0.70f, 0.25f, 1.0f));
        renderer.DrawQuad(badgePos, glm::vec2(badgeWidth, 19.0f),
                          glm::vec4(0.18f, 0.22f, 0.35f, 1.0f)); // Royal blue backing

        renderer.DrawString(currentSpeaker, badgePos + glm::vec2(10.0f, 4.0f),
                            1.0f, glm::vec4(1.0f, 0.95f, 0.35f, 1.0f));
    }

    // 3. Dialogue Text Lines
    if (!textLine1.empty()) {
        renderer.DrawString(textLine1, boxPos + glm::vec2(25.0f, 24.0f),
                            1.15f, glm::vec4(0.96f, 0.96f, 0.98f, 1.0f));
    }
    if (!textLine2.empty()) {
        renderer.DrawString(textLine2, boxPos + glm::vec2(25.0f, 52.0f),
                            1.10f, glm::vec4(0.90f, 0.90f, 0.94f, 1.0f));
    }

    // 4. Skip prompt hint
    renderer.DrawString("[SPACE] TO SKIP", boxPos + glm::vec2(boxWidth - 145.0f, boxHeight - 20.0f),
                        0.75f, glm::vec4(0.60f, 0.65f, 0.75f, 0.85f));
}

