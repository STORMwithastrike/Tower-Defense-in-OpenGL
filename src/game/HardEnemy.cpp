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

    // 1. Head: Matches MeleePlayer head + minor change (single triangle hat)
    renderer.DrawQuad(position + glm::vec2(-26.0f, -19.0f), glm::vec2(16.0f, 14.0f),
                      glm::vec4(0.50f, 0.50f, 0.55f, 1.0f), 0.0f, red);
    renderer.DrawTriangle(position + glm::vec2(-26.0f, -19.0f),
                          position + glm::vec2(-10.0f, -19.0f),
                          position + glm::vec2(-18.0f, -27.0f),
                          glm::vec4(0.75f, 0.15f, 0.15f, 1.0f), 0.0f, red);

    // 2. Body: Matches MeleePlayer body geometry with enemy deep red clothing
    renderer.DrawQuad(position + glm::vec2(-28.0f, -5.0f), glm::vec2(20.0f, 24.0f),
                      glm::vec4(0.75f, 0.15f, 0.15f, 1.0f), 0.0f, red);

    // 3. Weapon: Battleaxe with minor change (2 basic shapes total: 1 handle quad + 1 axe blade triangle)
    float slashOffset = (swingAnimTimer > 0.0f) ? -6.0f : 0.0f;
    // Shape 1: Handle quad
    renderer.DrawQuad(position + glm::vec2(-42.0f, 3.0f + slashOffset), glm::vec2(14.0f, 4.0f),
                      glm::vec4(0.50f, 0.35f, 0.25f, 1.0f), 0.0f, red);
    // Shape 2: Axe blade triangle pointing left
    renderer.DrawTriangle(position + glm::vec2(-42.0f, -5.0f + slashOffset),
                          position + glm::vec2(-42.0f, 13.0f + slashOffset),
                          position + glm::vec2(-50.0f, 4.0f + slashOffset),
                          glm::vec4(0.85f, 0.85f, 0.90f, 1.0f), 0.0f, red);

    // 4. Visible Health Bar above enemy
    renderHealthBar(renderer, -30.0f);
}
