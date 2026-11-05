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

  // 1. Head: Matches DefensePlayer head + minor change (single triangle hat)
  renderer.DrawQuad(position + glm::vec2(-24.0f, -21.0f), glm::vec2(16.0f, 12.0f),
                    glm::vec4(0.40f, 0.42f, 0.48f, 1.0f), 0.0f, red);
  renderer.DrawTriangle(position + glm::vec2(-24.0f, -21.0f),
                        position + glm::vec2(-8.0f, -21.0f),
                        position + glm::vec2(-16.0f, -28.0f),
                        glm::vec4(0.75f, 0.20f, 0.15f, 1.0f), 0.0f, red);

  // 2. Body: Matches DefensePlayer body geometry with enemy red clothing
  renderer.DrawQuad(position + glm::vec2(-24.0f, -9.0f), glm::vec2(18.0f, 24.0f),
                    glm::vec4(0.75f, 0.20f, 0.15f, 1.0f), 0.0f, red);

  // 3. Shield: Matches DefensePlayer shield + minor change (front spike, 3 basic shapes total)
  float recoil = (thrustAnim > 0.0f) ? 3.0f : 0.0f;
  // Shape 1: Shield body quad
  renderer.DrawQuad(position + glm::vec2(-36.0f + recoil, -19.0f), glm::vec2(12.0f, 32.0f),
                    glm::vec4(0.50f, 0.52f, 0.58f, 1.0f), 0.0f, red);
  // Shape 2: Bottom triangle (identical to DefensePlayer shield bottom)
  renderer.DrawTriangle(position + glm::vec2(-36.0f + recoil, 13.0f),
                        position + glm::vec2(-24.0f + recoil, 13.0f),
                        position + glm::vec2(-30.0f + recoil, 21.0f),
                        glm::vec4(0.50f, 0.52f, 0.58f, 1.0f), 0.0f, red);
  // Shape 3: Low front spike triangle pointing left (signals low attacks)
  renderer.DrawTriangle(position + glm::vec2(-36.0f + recoil, 7.0f),
                        position + glm::vec2(-36.0f + recoil, 17.0f),
                        position + glm::vec2(-44.0f + recoil, 12.0f),
                        glm::vec4(0.75f, 0.20f, 0.15f, 1.0f), 0.0f, red);

  // 4. Visible Health Bar above enemy
  renderHealthBar(renderer, -31.0f);
}
