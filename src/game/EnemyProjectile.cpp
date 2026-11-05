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
    return BoundingBox(position.x - 20.0f, position.y - 6.0f, 32.0f, 12.0f);
}

void EnemyProjectile::Render(Renderer& renderer) {
    if (!active) return;

    // Matches the ranged player's arrow projectile model (wooden shaft quad + iron arrowhead triangle, facing left)
    // 1. Shaft (wood stick)
    renderer.DrawQuad(position + glm::vec2(-12.0f, -3.0f), glm::vec2(26.0f, 6.0f),
                      glm::vec4(0.55f, 0.35f, 0.20f, 1.0f));

    // 2. Arrowhead (iron tip pointing left towards players)
    renderer.DrawTriangle(position + glm::vec2(-12.0f, -6.0f),
                          position + glm::vec2(-12.0f, 6.0f),
                          position + glm::vec2(-24.0f, 0.0f),
                          glm::vec4(0.20f, 0.20f, 0.22f, 1.0f));
}

