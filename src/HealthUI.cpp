#include "../headers/HealthUI.h"
#include "render/Renderer.h"
#include "render/VertexArray.h"

void HealthUI::draw(Renderer &renderer, glm::vec2 entityPos, float entityWidth,
                    int currentHp, int maxHp, float yOffset, float barWidth,
                    float barHeight) {
  if (currentHp <= 0 || maxHp <= 0) {
    return;
  }

  // Center el health bar above el entity
  glm::vec2 barPos(entityPos.x + (entityWidth - barWidth) / 2.0f,
                   entityPos.y + yOffset);
  renderer.DrawHealthBar(barPos, glm::vec2(barWidth, barHeight), currentHp,
                         maxHp, true);
}

void HealthUI::draw(Renderer &renderer, VertexArray &quadVAO,
                    glm::vec2 entityPos, float entityWidth, int currentHp,
                    int maxHp) {
  (void)quadVAO;
  draw(renderer, entityPos, entityWidth, currentHp, maxHp, -12.0f, 40.0f, 6.0f);
}
