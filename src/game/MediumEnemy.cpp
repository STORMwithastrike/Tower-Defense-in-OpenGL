#include "MediumEnemy.h"
#include <cmath>

MediumEnemy::MediumEnemy(int lane, const glm::vec2 &startPos)
    : Enemy(lane, startPos, EnemyType::MediumLowerRanged, 5, 1, 35.0f),
      thrustAnim(0.0f) {
  shootInterval = 3.2f;
  shootTimer = 1.0f;
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

      glm::vec2 spawnPos = position + glm::vec2(-26.0f, 16.0f);
      activeProjectiles.emplace_back(lane, spawnPos, -150.0f, AttackHeight::LOW,
                                     damage);
    }
  }
}

void MediumEnemy::Render(Renderer &renderer) {
  float red = GetDamageIntensity();

  renderer.DrawQuad(position + glm::vec2(-21.0f, -11.0f), glm::vec2(12.0f, 11.0f),
                    glm::vec4(0.30f, 0.30f, 0.35f, 1.0f), 0.0f, red);
  renderer.DrawTriangle(position + glm::vec2(-21.0f, -11.0f),
                        position + glm::vec2(-9.0f, -11.0f),
                        position + glm::vec2(-15.0f, -17.0f),
                        glm::vec4(0.75f, 0.18f, 0.18f, 1.0f), 0.0f, red);

  renderer.DrawQuad(position + glm::vec2(-22.0f, 0.0f), glm::vec2(14.0f, 18.0f),
                    glm::vec4(0.75f, 0.18f, 0.18f, 1.0f), 0.0f, red);

  float recoil = (thrustAnim > 0.0f) ? 2.5f : 0.0f;
  renderer.DrawQuad(position + glm::vec2(-36.0f + recoil, 7.0f), glm::vec2(14.0f, 3.0f),
                    glm::vec4(0.24f, 0.14f, 0.08f, 1.0f), 0.0f, red);
  renderer.DrawQuad(position + glm::vec2(-34.0f + recoil, 1.0f), glm::vec2(3.0f, 15.0f),
                    glm::vec4(0.30f, 0.30f, 0.35f, 1.0f), 0.0f, red);
  renderer.DrawTriangle(position + glm::vec2(-36.0f + recoil, 5.5f),
                        position + glm::vec2(-36.0f + recoil, 11.5f),
                        position + glm::vec2(-41.0f + recoil, 8.5f),
                        glm::vec4(0.75f, 0.18f, 0.18f, 1.0f), 0.0f, red);

  renderHealthBar(renderer, -21.0f);
}
