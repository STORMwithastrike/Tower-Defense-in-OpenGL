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

    // 1. Head (matches MeleePlayer head model)
    renderer.DrawQuad(position + glm::vec2(-26.0f, -19.0f), glm::vec2(16.0f, 14.0f),
                      glm::vec4(0.60f, 0.65f, 0.70f, 1.0f), 0.0f, red);

    // 2. Body (matches MeleePlayer body geometry, with enemy deep red clothing)
    renderer.DrawQuad(position + glm::vec2(-28.0f, -5.0f), glm::vec2(20.0f, 24.0f),
                      glm::vec4(0.75f, 0.15f, 0.15f, 1.0f), 0.0f, red);

    // 3. Sword (matches MeleePlayer sword, pointing left towards players)
    float slashOffset = (swingAnimTimer > 0.0f) ? -4.0f : 0.0f;
    renderer.DrawQuad(position + glm::vec2(-42.0f, 3.0f + slashOffset), glm::vec2(14.0f, 4.0f),
                      glm::vec4(0.85f, 0.85f, 0.90f, 1.0f), 0.0f, red);
    renderer.DrawTriangle(position + glm::vec2(-42.0f, 1.0f + slashOffset),
                          position + glm::vec2(-42.0f, 9.0f + slashOffset),
                          position + glm::vec2(-48.0f, 5.0f + slashOffset),
                          glm::vec4(0.85f, 0.85f, 0.90f, 1.0f), 0.0f, red);

    // 4. Visible Health Bar above enemy
    renderHealthBar(renderer, -28.0f);
}
