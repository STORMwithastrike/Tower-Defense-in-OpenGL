#include "MediumEnemy.h"
#include <cmath>

MediumEnemy::MediumEnemy(int lane, const glm::vec2& startPos)
    : Enemy(lane, startPos, EnemyType::MediumLowerRanged, 5, 1, 32.0f),
      thrustAnim(0.0f)
{
    shootInterval = 3.6f;
    shootTimer = 1.8f;
}

void MediumEnemy::Update(float dt, std::vector<EnemyProjectile>& activeProjectiles) {
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
            activeProjectiles.emplace_back(lane, spawnPos, -145.0f, AttackHeight::LOW, damage);
        }
    }
}

void MediumEnemy::Render(Renderer& renderer) {
    float red = GetDamageIntensity();
    float legSwing = sinf(walkAnimTimer) * 4.0f;

    // 1. Stocky Legs & Heavy Iron Sabatons
    renderer.DrawQuad(position + glm::vec2(-8.0f + legSwing, 12.0f), glm::vec2(6.0f, 13.0f),
                      glm::vec4(0.35f, 0.35f, 0.38f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(-11.0f + legSwing, 22.0f), glm::vec2(9.0f, 5.0f),
                      glm::vec4(0.20f, 0.20f, 0.22f, 1.0f), 0.0f, red);

    renderer.DrawQuad(position + glm::vec2(2.0f - legSwing, 12.0f), glm::vec2(6.0f, 13.0f),
                      glm::vec4(0.30f, 0.30f, 0.33f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(-1.0f - legSwing, 22.0f), glm::vec2(9.0f, 5.0f),
                      glm::vec4(0.20f, 0.20f, 0.22f, 1.0f), 0.0f, red);

    // 2. Heavy Broad Plate Torso & Gorget
    renderer.DrawQuad(position + glm::vec2(-13.0f, -8.0f), glm::vec2(26.0f, 22.0f),
                      glm::vec4(0.42f, 0.42f, 0.45f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(-10.0f, -4.0f), glm::vec2(20.0f, 3.0f),
                      glm::vec4(0.22f, 0.22f, 0.25f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(-10.0f, 2.0f), glm::vec2(20.0f, 3.0f),
                      glm::vec4(0.22f, 0.22f, 0.25f, 1.0f), 0.0f, red);

    // 3. Kettle-Hat Helmet
    renderer.DrawQuad(position + glm::vec2(-9.0f, -22.0f), glm::vec2(18.0f, 14.0f),
                      glm::vec4(0.48f, 0.48f, 0.52f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(-15.0f, -14.0f), glm::vec2(30.0f, 4.0f),
                      glm::vec4(0.30f, 0.30f, 0.34f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(-8.0f, -17.0f), glm::vec2(12.0f, 2.5f),
                      glm::vec4(0.05f, 0.05f, 0.08f, 1.0f), 0.0f, red);

    // 4. Heavy Pavise Shield
    renderer.DrawQuad(position + glm::vec2(7.0f, -12.0f), glm::vec2(8.0f, 30.0f),
                      glm::vec4(0.55f, 0.15f, 0.15f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(7.0f, -12.0f), glm::vec2(8.0f, 3.0f),
                      glm::vec4(0.25f, 0.25f, 0.28f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(7.0f, 15.0f), glm::vec2(8.0f, 3.0f),
                      glm::vec4(0.25f, 0.25f, 0.28f, 1.0f), 0.0f, red);

    // 5. Ground-Level Low Launcher Mechanism
    float lThrust = (thrustAnim > 0.0f) ? -5.0f : 0.0f;
    renderer.DrawQuad(position + glm::vec2(-24.0f + lThrust, 14.0f), glm::vec2(16.0f, 8.0f),
                      glm::vec4(0.35f, 0.22f, 0.12f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(-28.0f + lThrust, 15.0f), glm::vec2(10.0f, 4.0f),
                      glm::vec4(0.20f, 0.20f, 0.22f, 1.0f), 0.0f, red);
    renderer.DrawCircle(position + glm::vec2(-14.0f + lThrust, 22.0f), 4.0f,
                        glm::vec4(0.25f, 0.25f, 0.28f, 1.0f), 8, red);

    // 6. Visible Health Bar above helmet
    renderHealthBar(renderer, -36.0f);
}
