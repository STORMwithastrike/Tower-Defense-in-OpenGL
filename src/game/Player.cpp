#include "Player.h"
#include <algorithm>
#include <cmath>

Player::Player(int lane, const glm::vec2& startPos, PlayerType type, int maxHealth, float speed)
    : lane(lane),
      position(startPos),
      speed(speed),
      type(type),
      health(maxHealth),
      maxHealth(maxHealth),
      jumping(false),
      jumpTimer(0.0f),
      jumpOffset(0.0f),
      ducking(false),
      duckTimer(0.0f),
      fighting(false),
      attackTimer(0.0f),
      attackCooldown(1.0f),
      damageFlashTimer(0.0f),
      walkAnimTimer(0.0f)
{
}

void Player::Jump() {
    if (IsStationary() || jumping || ducking) return;
    jumping = true;
    jumpTimer = 0.55f; // Jump duration in seconds
}

void Player::Duck() {
    if (IsStationary() || jumping || ducking) return;
    ducking = true;
    duckTimer = 0.55f; // Duck duration in seconds
}

void Player::updateDodging(float dt) {
    if (jumping) {
        jumpTimer -= dt;
        if (jumpTimer <= 0.0f) {
            jumping = false;
            jumpOffset = 0.0f;
        } else {
            // Parabolic jump arc: peak around 26 pixels high
            float progress = 1.0f - (jumpTimer / 0.55f);
            jumpOffset = sinf(progress * 3.14159265f) * 26.0f;
        }
    }

    if (ducking) {
        duckTimer -= dt;
        if (duckTimer <= 0.0f) {
            ducking = false;
        }
    }
}

void Player::Update(float dt) {
    if (damageFlashTimer > 0.0f) {
        damageFlashTimer -= dt;
    }

    updateDodging(dt);

    if (!fighting && !IsStationary()) {
        position.x += speed * dt;
        walkAnimTimer += dt * 6.0f;
    }

    if (fighting) {
        attackTimer += dt;
    }
}

void Player::TakeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
    damageFlashTimer = 0.25f; // Red hit flash
}

BoundingBox Player::GetBounds() const {
    float effectiveY = position.y - jumpOffset;
    float h = ducking ? 24.0f : 44.0f;
    float topY = ducking ? effectiveY + 16.0f : effectiveY;
    return BoundingBox(position.x - 14.0f, topY - 22.0f, 28.0f, h);
}

