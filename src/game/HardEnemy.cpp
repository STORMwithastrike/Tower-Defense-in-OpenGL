#include "HardEnemy.h"
#include <cmath>

HardEnemy::HardEnemy(int lane, const glm::vec2& startPos)
    : Enemy(lane, startPos, EnemyType::HardMelee, 7, 2, 68.0f),
      swordAngle(0.0f),
      swingAnimTimer(0.0f)
{
    attackCooldown = 0.9f;
}

void HardEnemy::TriggerSlash() {
    swingAnimTimer = 0.45f;
}

void HardEnemy::Update(float dt, std::vector<EnemyProjectile>& activeProjectiles) {
    Enemy::Update(dt, activeProjectiles);

    if (swingAnimTimer > 0.0f) {
        swingAnimTimer -= dt;
    }

    if (fighting) {
        if (attackTimer >= attackCooldown) {
            TriggerSlash();
        }
    }
}

void HardEnemy::Render(Renderer& renderer) {
    float red = GetDamageIntensity();
    float legSwing = sinf(walkAnimTimer * 1.5f) * 7.0f;

    // 1. Spiked Iron Greaves & Heavy War-Boots
    renderer.DrawQuad(position + glm::vec2(-8.0f + legSwing, 14.0f), glm::vec2(7.0f, 15.0f),
                      glm::vec4(0.20f, 0.20f, 0.22f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(-11.0f + legSwing, 25.0f), glm::vec2(10.0f, 5.0f),
                      glm::vec4(0.14f, 0.14f, 0.16f, 1.0f), 0.0f, red);

    renderer.DrawQuad(position + glm::vec2(3.0f - legSwing, 14.0f), glm::vec2(7.0f, 15.0f),
                      glm::vec4(0.25f, 0.25f, 0.28f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(0.0f - legSwing, 25.0f), glm::vec2(10.0f, 5.0f),
                      glm::vec4(0.14f, 0.14f, 0.16f, 1.0f), 0.0f, red);

    // 2. Imposing Heavy Dark Plate Torso & Crimson Tabard
    renderer.DrawQuad(position + glm::vec2(-14.0f, -10.0f), glm::vec2(28.0f, 25.0f),
                      glm::vec4(0.22f, 0.22f, 0.24f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(-8.0f, -6.0f), glm::vec2(16.0f, 18.0f),
                      glm::vec4(0.55f, 0.10f, 0.10f, 1.0f), 0.0f, red);

    // 3. Spiked Shoulder Pauldrons
    renderer.DrawTriangle(position + glm::vec2(-14.0f, -18.0f),
                          position + glm::vec2(-22.0f, -4.0f),
                          position + glm::vec2(-10.0f, -4.0f),
                          glm::vec4(0.32f, 0.32f, 0.36f, 1.0f), 0.0f, red);
    renderer.DrawTriangle(position + glm::vec2(-18.0f, -24.0f),
                          position + glm::vec2(-21.0f, -16.0f),
                          position + glm::vec2(-14.0f, -16.0f),
                          glm::vec4(0.18f, 0.18f, 0.20f, 1.0f), 0.0f, red);

    renderer.DrawTriangle(position + glm::vec2(14.0f, -18.0f),
                          position + glm::vec2(10.0f, -4.0f),
                          position + glm::vec2(22.0f, -4.0f),
                          glm::vec4(0.32f, 0.32f, 0.36f, 1.0f), 0.0f, red);
    renderer.DrawTriangle(position + glm::vec2(18.0f, -24.0f),
                          position + glm::vec2(14.0f, -16.0f),
                          position + glm::vec2(21.0f, -16.0f),
                          glm::vec4(0.18f, 0.18f, 0.20f, 1.0f), 0.0f, red);

    // 4. Horned Great-Helm
    renderer.DrawQuad(position + glm::vec2(-10.0f, -26.0f), glm::vec2(20.0f, 16.0f),
                      glm::vec4(0.28f, 0.28f, 0.32f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(-11.0f, -20.0f), glm::vec2(14.0f, 3.0f),
                      glm::vec4(1.0f, 0.15f, 0.15f, 1.0f), 0.0f, red);
    renderer.DrawTriangle(position + glm::vec2(-16.0f, -36.0f),
                          position + glm::vec2(-10.0f, -26.0f),
                          position + glm::vec2(-6.0f, -26.0f),
                          glm::vec4(0.15f, 0.15f, 0.16f, 1.0f), 0.0f, red);
    renderer.DrawTriangle(position + glm::vec2(16.0f, -36.0f),
                          position + glm::vec2(6.0f, -26.0f),
                          position + glm::vec2(10.0f, -26.0f),
                          glm::vec4(0.15f, 0.15f, 0.16f, 1.0f), 0.0f, red);

    // 5. Giant Executioner Greatsword
    float slashOffset = 0.0f;
    if (swingAnimTimer > 0.0f) {
        float p = 1.0f - (swingAnimTimer / 0.45f);
        slashOffset = sinf(p * 3.14159265f) * 22.0f;
    }
    renderer.DrawQuad(position + glm::vec2(-26.0f, -12.0f + slashOffset), glm::vec2(5.0f, 20.0f),
                      glm::vec4(0.70f, 0.55f, 0.20f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(-22.0f, -4.0f + slashOffset), glm::vec2(8.0f, 4.0f),
                      glm::vec4(0.20f, 0.14f, 0.10f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(-44.0f, -8.0f + slashOffset), glm::vec2(18.0f, 12.0f),
                      glm::vec4(0.75f, 0.78f, 0.82f, 1.0f), 0.0f, red);
    renderer.DrawTriangle(position + glm::vec2(-52.0f, -2.0f + slashOffset),
                          position + glm::vec2(-44.0f, -8.0f + slashOffset),
                          position + glm::vec2(-44.0f, 4.0f + slashOffset),
                          glm::vec4(0.75f, 0.78f, 0.82f, 1.0f), 0.0f, red);

    // 6. Visible Health Bar above horns
    renderHealthBar(renderer, -44.0f);
}
