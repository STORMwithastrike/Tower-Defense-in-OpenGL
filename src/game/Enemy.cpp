#include "Enemy.h"
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
    damageFlashTimer = 0.28f; // Flash red
}

float Enemy::GetDamageIntensity() const {
    // 1. Base red intensity increases as health decreases (from 0.0 at full health to 0.7 at 1 HP)
    float healthDeficit = 1.0f - ((float)health / (float)maxHealth);
    float baseRed = healthDeficit * 0.70f;

    // 2. Add quick flash when hit
    if (damageFlashTimer > 0.0f) {
        baseRed = std::max(baseRed, 0.85f);
    }

    return std::min(1.0f, baseRed);
}

void Enemy::Update(float dt, std::vector<Projectile>& activeProjectiles) {
    (void)activeProjectiles; // Base update; specialized ranged enemies override to fire projectiles

    if (damageFlashTimer > 0.0f) {
        damageFlashTimer -= dt;
    }

    if (!fighting) {
        // Move towards home base on the left (negative X)
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
    glm::vec2 barPos(position.x - 18.0f, position.y + yOffset);
    glm::vec2 barSize(36.0f, 5.0f);
    renderer.DrawHealthBar(barPos, barSize, health, maxHealth, true);
}

