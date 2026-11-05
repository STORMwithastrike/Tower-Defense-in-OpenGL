#include "Collectible.h"
#include <math.h>

Collectible::Collectible(int lane, const glm::vec2& pos, int value) {
    this->lane = lane;
    position = pos;
    this->value = value;
    active = true;
    animTimer = 0.0f;
}

void Collectible::Update(float dt) {
    if (active) {
        animTimer += dt;
    }
}

BoundingBox Collectible::GetBounds() const {
    float bx = position.x - 12.0f;
    float by = position.y - 12.0f;
    return BoundingBox(bx, by, 24.0f, 24.0f);
}

void Collectible::Render(Renderer& renderer) {
    if (active) {
        float yOffset = (float)sin(animTimer * 4.0f) * 4.0f;
        glm::vec2 p = glm::vec2(position.x, position.y + yOffset);

        renderer.DrawCircle(p, 11.0f, glm::vec4(0.85f, 0.65f, 0.12f, 1.0f), 16);
        renderer.DrawCircle(p, 8.5f, glm::vec4(1.0f, 0.85f, 0.25f, 1.0f), 16);

        renderer.DrawQuad(glm::vec2(p.x - 3.0f, p.y - 3.0f), glm::vec2(6.0f, 6.0f), glm::vec4(0.75f, 0.5f, 0.08f, 1.0f));

        glm::vec2 p1 = glm::vec2(p.x, p.y - 6.0f);
        glm::vec2 p2 = glm::vec2(p.x - 4.0f, p.y - 1.0f);
        glm::vec2 p3 = glm::vec2(p.x + 4.0f, p.y - 1.0f);
        renderer.DrawTriangle(p1, p2, p3, glm::vec4(0.95f, 0.8f, 0.15f, 1.0f));
    }
}