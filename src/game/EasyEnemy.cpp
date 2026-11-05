#include "EasyEnemy.h"
#include <cmath>

EasyEnemy::EasyEnemy(int lane, const glm::vec2 &startPos)
    : Enemy(lane, startPos, EnemyType::EasyUpperRanged, 5, 1, 35.0f),
      reloadAnim(0.0f) {
  shootInterval = 3.2f;
  shootTimer = 1.0f;
}

void EasyEnemy::Update(float dt,
                       std::vector<EnemyProjectile> &activeProjectiles) {
  Enemy::Update(dt, activeProjectiles);

  if (reloadAnim > 0.0f) {
    reloadAnim -= dt;
  }

  if (!fighting) {
    shootTimer += dt;
    if (shootTimer >= shootInterval) {
      shootTimer = 0.0f;
      reloadAnim = 0.6f;

      // Fires a HIGH projectile (originates high, travels across upper half of
      // the lane) Player must DUCK to avoid this attack!
      glm::vec2 spawnPos = position + glm::vec2(-24.0f, -14.0f);
      activeProjectiles.emplace_back(lane, spawnPos, -150.0f,
                                     AttackHeight::HIGH, damage);
    }
  }
}

void EasyEnemy::Render(Renderer &renderer) {
  float red = GetDamageIntensity();

  // 1. Head (matches player model)
  renderer.DrawQuad(position + glm::vec2(-25.0f, -19.0f), glm::vec2(16.0f, 14.0f),
                    glm::vec4(0.30f, 0.30f, 0.35f, 1.0f), 0.0f, red);

  // 2. Body (matches player model geometry, with enemy red clothing)
  renderer.DrawQuad(position + glm::vec2(-26.0f, -5.0f), glm::vec2(18.0f, 24.0f),
                    glm::vec4(0.75f, 0.18f, 0.18f, 1.0f), 0.0f, red);

  // 3. Crossbow (high position for upper ranged attacks, pointing left)
  float recoil = (reloadAnim > 0.0f) ? 3.0f : 0.0f;
  renderer.DrawQuad(position + glm::vec2(-44.0f + recoil, -7.0f), glm::vec2(18.0f, 4.0f),
                    glm::vec4(0.24f, 0.14f, 0.08f, 1.0f), 0.0f, red);
  renderer.DrawQuad(position + glm::vec2(-42.0f + recoil, -15.0f), glm::vec2(4.0f, 20.0f),
                    glm::vec4(0.30f, 0.30f, 0.35f, 1.0f), 0.0f, red);

  // 4. Visible Health Bar above enemy
  renderHealthBar(renderer, -28.0f);
}
