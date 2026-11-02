#include "EasyEnemy.h"
#include <cmath>

EasyEnemy::EasyEnemy(int lane, const glm::vec2& startPos)
    : Enemy(lane, startPos, EnemyType::EASY_UPPER_RANGED, 5, 1, 35.0f),
      reloadAnim(0.0f)
{
    shootInterval = 3.2f;
    shootTimer = 1.0f; // Initial delay before first volley
}

void EasyEnemy::Update(float dt, std::vector<Projectile>& activeProjectiles) {
    Enemy::Update(dt, activeProjectiles);

    if (reloadAnim > 0.0f) {
        reloadAnim -= dt;
    }

    if (!fighting) {
        shootTimer += dt;
        if (shootTimer >= shootInterval) {
            shootTimer = 0.0f;
            reloadAnim = 0.6f;

            // Fires a HIGH projectile (originates high, travels across upper half of the lane)
            // Player must DUCK to avoid this attack!
            glm::vec2 spawnPos = position + glm::vec2(-24.0f, -14.0f);
            activeProjectiles.emplace_back(lane, spawnPos, -150.0f, false, AttackHeight::HIGH, damage);
        }
    }
}

void EasyEnemy::Render(Renderer& renderer) {
    float red = GetDamageIntensity();

    // Walking stride oscillation
    float legSwing = sinf(walkAnimTimer) * 5.0f;

    // 1. Legs & Boots (Composite iron greaves)
    // Left Leg
    renderer.DrawQuad(position + glm::vec2(-6.0f + legSwing, 10.0f), glm::vec2(5.0f, 14.0f),
                      glm::vec4(0.28f, 0.28f, 0.30f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(-8.0f + legSwing, 22.0f), glm::vec2(7.0f, 4.0f),
                      glm::vec4(0.20f, 0.18f, 0.16f, 1.0f), 0.0f, red); // Boot
    // Right Leg
    renderer.DrawQuad(position + glm::vec2(2.0f - legSwing, 10.0f), glm::vec2(5.0f, 14.0f),
                      glm::vec4(0.32f, 0.32f, 0.35f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(0.0f - legSwing, 22.0f), glm::vec2(7.0f, 4.0f),
                      glm::vec4(0.20f, 0.18f, 0.16f, 1.0f), 0.0f, red); // Boot

    // 2. Torso (Chainmail undergarment + Dark Crimson Surcoat)
    renderer.DrawQuad(position + glm::vec2(-10.0f, -10.0f), glm::vec2(20.0f, 22.0f),
                      glm::vec4(0.65f, 0.12f, 0.12f, 1.0f), 0.0f, red); // Crimson surcoat
    // Leather Belt & Buckle
    renderer.DrawQuad(position + glm::vec2(-11.0f, 4.0f), glm::vec2(22.0f, 4.0f),
                      glm::vec4(0.22f, 0.15f, 0.10f, 1.0f), 0.0f, red);
    renderer.DrawQuad(position + glm::vec2(-2.0f, 3.5f), glm::vec2(4.0f, 5.0f),
                      glm::vec4(0.85f, 0.70f, 0.20f, 1.0f), 0.0f, red); // Gold buckle

    // 3. Head & Helmet (Bascinet Helm with Visor and Crimson Plume)
    renderer.DrawQuad(position + glm::vec2(-8.0f, -24.0f), glm::vec2(16.0f, 14.0f),
                      glm::vec4(0.40f, 0.40f, 0.44f, 1.0f), 0.0f, red); // Steel dome
    // Visor eye slit
    renderer.DrawQuad(position + glm::vec2(-9.0f, -19.0f), glm::vec2(10.0f, 3.0f),
                      glm::vec4(0.08f, 0.08f, 0.10f, 1.0f), 0.0f, red);
    // Crimson Feather Plume atop helmet
    renderer.DrawTriangle(position + glm::vec2(0.0f, -31.0f),
                          position + glm::vec2(-4.0f, -24.0f),
                          position + glm::vec2(4.0f, -24.0f),
                          glm::vec4(0.85f, 0.15f, 0.15f, 1.0f), 0.0f, red);

    // 4. Crossbow / Arbalest (Raised high at an angle to signal high shots)
    float recoil = (reloadAnim > 0.0f) ? 4.0f : 0.0f;
    // Wooden Stock
    renderer.DrawQuad(position + glm::vec2(-22.0f + recoil, -14.0f), glm::vec2(16.0f, 4.0f),
                      glm::vec4(0.45f, 0.28f, 0.15f, 1.0f), 0.0f, red);
    // Steel Bow Prods (Vertical cross-limbs)
    renderer.DrawQuad(position + glm::vec2(-23.0f + recoil, -24.0f), glm::vec2(3.5f, 22.0f),
                      glm::vec4(0.35f, 0.35f, 0.38f, 1.0f), 0.0f, red);
    // Bowstring
    renderer.DrawQuad(position + glm::vec2(-14.0f + recoil, -22.0f), glm::vec2(1.5f, 18.0f),
                      glm::vec4(0.80f, 0.80f, 0.80f, 1.0f), 0.0f, red);

    // 5. Visible Health Bar above helmet
    renderHealthBar(renderer, -38.0f);
}

