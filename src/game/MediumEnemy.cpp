#include "MediumEnemy.h"
#include <cmath>

MediumEnemy::MediumEnemy(int lane, const glm::vec2 &startPos)
    : Enemy(lane, startPos, EnemyType::MediumLowerRanged, 5, 1, 32.0f),
      thrustAnim(0.0f) {
  shootInterval = 3.6f;
  shootTimer = 1.8f;
}

void MediumEnemy::Update(float dt,
                         std::vector<EnemyProjectile> &activeProjectiles) {
  Enemy::Update(dt, activeProjectiles);

  if (thrustAnim > 0.0f) {
    thrustAnim -= dt;
  }

  if (!fighting) {
    shootTimer += dt;
    if (shootTimer >= shootInterval) {
      shootTimer = 0.0f;
      thrustAnim = 0.5f;

      // Fires a LOW projectile along the floor of the lane
      // Player must JUMP to avoid this attack!
      glm::vec2 spawnPos = position + glm::vec2(-26.0f, 16.0f);
      activeProjectiles.emplace_back(lane, spawnPos, -145.0f, AttackHeight::LOW,
                                     damage);
    }
  }
}

void MediumEnemy::Render(Renderer &renderer) {
  float red = GetDamageIntensity();

  // 1. Head (matches player model)
  renderer.DrawQuad(position + glm::vec2(-25.0f, -19.0f), glm::vec2(16.0f, 14.0f),
                    glm::vec4(0.30f, 0.30f, 0.35f, 1.0f), 0.0f, red);

  // 2. Body (matches player model geometry, with enemy orange-red clothing)
  renderer.DrawQuad(position + glm::vec2(-26.0f, -5.0f), glm::vec2(18.0f, 24.0f),
                    glm::vec4(0.80f, 0.30f, 0.15f, 1.0f), 0.0f, red);

  // 3. Crossbow (low position for lower ranged attacks, pointing left)
  float recoil = (thrustAnim > 0.0f) ? 3.0f : 0.0f;
  renderer.DrawQuad(position + glm::vec2(-44.0f + recoil, 11.0f), glm::vec2(18.0f, 4.0f),
                    glm::vec4(0.24f, 0.14f, 0.08f, 1.0f), 0.0f, red);
  renderer.DrawQuad(position + glm::vec2(-42.0f + recoil, 3.0f), glm::vec2(4.0f, 20.0f),
                    glm::vec4(0.30f, 0.30f, 0.35f, 1.0f), 0.0f, red);

  // 4. Visible Health Bar above enemy
  renderHealthBar(renderer, -28.0f);
}
