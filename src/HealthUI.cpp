#include "../headers/HealthUI.h"
#include "render/Renderer.h"
#include "render/VertexArray.h"

void HealthUI::draw(Renderer& renderer, VertexArray& quadVAO, glm::vec2 entityPos, float entityWidth, int currentHp, int maxHp) {
    //lw mat
    if (currentHp <= 0 || maxHp <= 0) {
        return;
    }

    // el dimentions
    const float barWidth = 40.0f;
    const float barHeight = 6.0f;

    //center fo2 el charcter
    glm::vec2 barPos(entityPos.x + (entityWidth - barWidth) / 2.0f, entityPos.y - 12.0f);

    // background
    renderer.Draw(quadVAO, barPos, glm::vec2(barWidth, barHeight), 1.0f, glm::vec4(0.15f, 0.15f, 0.15f, 1.0f));

    // health precnt
    float hpPercent = static_cast<float>(currentHp) / static_cast<float>(maxHp);
    if (hpPercent > 1.0f) hpPercent = 1.0f;
    if (hpPercent < 0.0f) hpPercent = 0.0f;

    // color for health
    glm::vec4 fillColor(0.15f, 0.85f, 0.25f, 1.0f); // Healthy
    if (hpPercent < 0.35f) {
        fillColor = glm::vec4(0.90f, 0.20f, 0.20f, 1.0f); // 5tar
    } else if (hpPercent < 0.65f) {
        fillColor = glm::vec4(0.95f, 0.80f, 0.15f, 1.0f); // ouch
    }

    //hp bar
    renderer.Draw(quadVAO, barPos, glm::vec2(barWidth * hpPercent, barHeight), 1.0f, fillColor);
}

