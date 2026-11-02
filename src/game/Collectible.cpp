#include "Collectible.h"
#include <cmath>

Collectible::Collectible(int lane, const glm::vec2& pos, int value)
    : lane(lane),
      position(pos),
      value(value),
      active(true),
      animTimer(0.0f)
{
}

void Collectible::Update(float dt) {
    if (!active) return;
    animTimer += dt;
}

BoundingBox Collectible::GetBounds() const {
    return BoundingBox(position.x - 12.0f, position.y - 12.0f, 24.0f, 24.0f);
}

void Collectible::Render(Renderer& renderer) {
    if (!active) return;

    // Bobbing / floating animation
    float bob = sinf(animTimer * 4.0f) * 4.0f;
    glm::vec2 drawPos = position + glm::vec2(0.0f, bob);

    // Composite Royal Gold Coin:
    // Outer golden rim
    renderer.DrawCircle(drawPos, 11.0f, glm::vec4(0.85f, 0.65f, 0.12f, 1.0f), 16);
    // Inner shiny face
    renderer.DrawCircle(drawPos, 8.5f, glm::vec4(1.0f, 0.85f, 0.25f, 1.0f), 16);
    // Embossed Royal Fleur/Crown symbol (composite mini-triangles & quad)
    renderer.DrawQuad(drawPos - glm::vec2(3.0f, 3.0f), glm::vec2(6.0f, 6.0f),
                      glm::vec4(0.75f, 0.50f, 0.08f, 1.0f));
    renderer.DrawTriangle(drawPos + glm::vec2(0.0f, -6.0f),
                          drawPos + glm::vec2(-4.0f, -1.0f),
                          drawPos + glm::vec2(4.0f, -1.0f),
                          glm::vec4(0.95f, 0.80f, 0.15f, 1.0f));
}

