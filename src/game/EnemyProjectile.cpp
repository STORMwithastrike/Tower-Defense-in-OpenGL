#include "EnemyProjectile.h"
#include <cmath>

EnemyProjectile::EnemyProjectile(int lane, const glm::vec2& pos, float speed, AttackHeight height, int damage)
    : lane(lane),
      position(pos),
      speed(speed),
      attackHeight(height),
      damage(damage),
      active(true),
      animTimer(0.0f)
{
}

void EnemyProjectile::Update(float dt) {
    if (!active) return;
    animTimer += dt;
    position.x += speed * dt;

    if (position.x < -50.0f || position.x > 850.0f) {
        active = false;
    }
}

BoundingBox EnemyProjectile::GetBounds() const {
    return BoundingBox(position.x - 8.0f, position.y - 8.0f, 16.0f, 16.0f);
}

void EnemyProjectile::Render(Renderer& renderer) {
    if (!active) return;

    if (attackHeight == AttackHeight::HIGH) {
        // Upper Ranged: Fiery Arbalest / Catapult Bomb (Player must duck!)
        float pulse = 0.5f + 0.5f * sinf(animTimer * 12.0f);
        renderer.DrawCircle(position, 8.0f + pulse * 2.0f, glm::vec4(0.95f, 0.35f, 0.1f, 1.0f), 12);
        renderer.DrawCircle(position, 4.5f, glm::vec4(1.0f, 0.9f, 0.2f, 1.0f), 10);
        renderer.DrawCircle(position + glm::vec2(7.0f, -2.0f), 3.0f, glm::vec4(0.85f, 0.15f, 0.1f, 0.7f), 8);
    } else {
        // Lower Ranged: Rolling Spiked Bomb / Low Spear (Player must jump!)
        renderer.DrawCircle(position, 7.5f, glm::vec4(0.25f, 0.25f, 0.28f, 1.0f), 12);
        renderer.DrawCircle(position, 3.5f, glm::vec4(0.75f, 0.20f, 0.15f, 1.0f), 8);
        renderer.DrawQuad(position + glm::vec2(3.0f, 4.0f), glm::vec2(6.0f, 3.0f),
                          glm::vec4(0.5f, 0.4f, 0.3f, 0.6f));
    }
}

