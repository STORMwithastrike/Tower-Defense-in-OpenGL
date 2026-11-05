#include "Enemy.h"
#include "../headers/HealthUI.h"
#include <algorithm>

Enemy::Enemy(int lane, const glm::vec2& startPos, EnemyType type, int maxHealth, int damage, float speed)
    : lane(lane),
      position(startPos),
      speed(speed),
      type(type),
      health(maxHealth),
      maxHealth(maxHealth),
      damage(damage),
      fighting(false),
      attackTimer(0.0f),
      attackCooldown(1.0f),
      damageFlashTimer(0.0f),
      walkAnimTimer(0.0f),
      shootTimer(0.0f),
      shootInterval(3.0f)
{
}

void Enemy::TakeDamage(int dmg) {
    health -= dmg;
    if (health < 0) health = 0;
    damageFlashTimer = 0.28f;
}

float Enemy::GetDamageIntensity() const {
    float healthDeficit = 1.0f - ((float)health / (float)maxHealth);
    float baseRed = healthDeficit * 0.70f;
    if (damageFlashTimer > 0.0f) {
        baseRed = std::max(baseRed, 0.85f);
    }
    return std::min(1.0f, baseRed);
}

void Enemy::Update(float dt, std::vector<EnemyProjectile>& activeProjectiles) {
    (void)activeProjectiles;

    if (damageFlashTimer > 0.0f) {
        damageFlashTimer -= dt;
    }

    if (!fighting) {
        position.x -= speed * dt;
        walkAnimTimer += dt * 6.0f;
    } else {
        attackTimer += dt;
    }
}

BoundingBox Enemy::GetBounds() const {
    return BoundingBox(position.x - 16.0f, position.y - 24.0f, 32.0f, 48.0f);
}

void Enemy::renderHealthBar(Renderer& renderer, float yOffset) const {
    HealthUI::draw(renderer, position, 0.0f, health, maxHealth, yOffset, 34.0f, 5.0f);
}
