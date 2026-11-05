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

    // Simple, clean textbox at bottom of screen
    float boxWidth = 720.0f;
    float boxHeight = 85.0f;
    glm::vec2 boxPos((screenWidth - boxWidth) * 0.5f, screenHeight - 100.0f);

    // 1. Clean background box with thin accent top border
    renderer.DrawQuad(boxPos, glm::vec2(boxWidth, boxHeight),
                      glm::vec4(0.08f, 0.10f, 0.15f, 0.90f));
    renderer.DrawQuad(boxPos, glm::vec2(boxWidth, 2.0f),
                      glm::vec4(0.85f, 0.70f, 0.20f, 0.90f));

    // 2. Speaker Name
    if (!currentSpeaker.empty()) {
        renderer.DrawString(currentSpeaker, boxPos + glm::vec2(20.0f, 12.0f),
                            0.95f, glm::vec4(1.0f, 0.88f, 0.25f, 1.0f));
    }

    // 3. Dialogue Text Lines
    if (!textLine1.empty()) {
        renderer.DrawString(textLine1, boxPos + glm::vec2(20.0f, 32.0f),
                            0.82f, glm::vec4(0.95f, 0.95f, 0.95f, 1.0f));
    }
    if (!textLine2.empty()) {
        renderer.DrawString(textLine2, boxPos + glm::vec2(20.0f, 52.0f),
                            0.82f, glm::vec4(0.85f, 0.88f, 0.92f, 1.0f));
    }

    // 4. Skip prompt
    renderer.DrawString("[SPACE] SKIP", boxPos + glm::vec2(boxWidth - 110.0f, 12.0f),
                        0.70f, glm::vec4(0.65f, 0.70f, 0.80f, 0.80f));
}

