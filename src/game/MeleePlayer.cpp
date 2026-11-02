#include "MeleePlayer.h"
#include <cmath>

MeleePlayer::MeleePlayer(int lane, const glm::vec2& startPos)
    : Player(lane, startPos, PlayerType::MELEE, 5, 38.0f),
      swingTimer(0.0f)
{
    attackCooldown = 0.95f;
}

void MeleePlayer::Update(float dt) {
    Player::Update(dt);

    if (swingTimer > 0.0f) {
        swingTimer -= dt;
    }

    if (fighting && attackTimer >= attackCooldown) {
        swingTimer = 0.40f;
    }
}

void MeleePlayer::Render(Renderer& renderer) {
    float red = (damageFlashTimer > 0.0f) ? 0.85f : 0.0f;
    float effectiveY = position.y - jumpOffset;

    // Walk oscillation
    float legSwing = sinf(walkAnimTimer) * 5.0f;
    float crouchOffset = ducking ? 12.0f : 0.0f;

    // 1. Legs & Sabatons
    renderer.DrawQuad(glm::vec2(position.x - 6.0f + legSwing, effectiveY + 10.0f + crouchOffset * 0.5f),
                      glm::vec2(5.0f, ducking ? 8.0f : 14.0f),
                      glm::vec4(0.48f, 0.50f, 0.55f, 1.0f), 0.0f, red);
    renderer.DrawQuad(glm::vec2(position.x + 2.0f - legSwing, effectiveY + 10.0f + crouchOffset * 0.5f),
                      glm::vec2(5.0f, ducking ? 8.0f : 14.0f),
                      glm::vec4(0.44f, 0.46f, 0.50f, 1.0f), 0.0f, red);

    // 2. Royal Blue Torso & Gold Crest
    renderer.DrawQuad(glm::vec2(position.x - 9.0f, effectiveY - 8.0f + crouchOffset),
                      glm::vec2(18.0f, ducking ? 15.0f : 20.0f),
                      glm::vec4(0.18f, 0.38f, 0.82f, 1.0f), 0.0f, red); // Royal Blue
    renderer.DrawQuad(glm::vec2(position.x - 3.0f, effectiveY - 4.0f + crouchOffset),
                      glm::vec2(6.0f, ducking ? 6.0f : 10.0f),
                      glm::vec4(0.95f, 0.80f, 0.20f, 1.0f), 0.0f, red); // Lion crest

    // 3. Silver Helmet & Blue Plume
    renderer.DrawQuad(glm::vec2(position.x - 7.0f, effectiveY - 22.0f + crouchOffset),
                      glm::vec2(14.0f, 13.0f),
                      glm::vec4(0.68f, 0.70f, 0.75f, 1.0f), 0.0f, red);
    // Eye slit
    renderer.DrawQuad(glm::vec2(position.x - 2.0f, effectiveY - 18.0f + crouchOffset),
                      glm::vec2(7.0f, 2.5f),
                      glm::vec4(0.10f, 0.12f, 0.16f, 1.0f), 0.0f, red);
    // Blue Feather Plume
    renderer.DrawTriangle(glm::vec2(position.x, effectiveY - 29.0f + crouchOffset),
                          glm::vec2(position.x - 4.0f, effectiveY - 22.0f + crouchOffset),
                          glm::vec2(position.x + 4.0f, effectiveY - 22.0f + crouchOffset),
                          glm::vec4(0.15f, 0.45f, 0.95f, 1.0f), 0.0f, red);

    // 4. Close-Range Sword (Animates in hand when attacking)
    float slashOffset = 0.0f;
    if (swingTimer > 0.0f) {
        float p = 1.0f - (swingTimer / 0.40f);
        slashOffset = sinf(p * 3.14159265f) * 18.0f;
    }
    // Crossguard & Hilt
    renderer.DrawQuad(glm::vec2(position.x + 12.0f, effectiveY - 4.0f + crouchOffset + slashOffset),
                      glm::vec2(3.0f, 12.0f), glm::vec4(0.85f, 0.70f, 0.20f, 1.0f), 0.0f, red);
    // Steel Blade
    renderer.DrawQuad(glm::vec2(position.x + 15.0f, effectiveY - 18.0f + crouchOffset + slashOffset),
                      glm::vec2(4.0f, 20.0f), glm::vec4(0.88f, 0.90f, 0.95f, 1.0f), 0.0f, red);
    renderer.DrawTriangle(glm::vec2(position.x + 17.0f, effectiveY - 25.0f + crouchOffset + slashOffset),
                          glm::vec2(position.x + 15.0f, effectiveY - 18.0f + crouchOffset + slashOffset),
                          glm::vec2(position.x + 19.0f, effectiveY - 18.0f + crouchOffset + slashOffset),
                          glm::vec4(0.95f, 0.95f, 1.0f, 1.0f), 0.0f, red);

    // 5. Visible Health Bar above helmet
    renderer.DrawHealthBar(glm::vec2(position.x - 16.0f, effectiveY - 36.0f + crouchOffset),
                           glm::vec2(32.0f, 5.0f), health, maxHealth, true);
}

