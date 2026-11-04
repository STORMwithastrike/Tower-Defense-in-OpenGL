#include "../headers/Player.h"

Player::Player(int maxHp, int lane)
    : Entity(maxHp, lane),
    health(maxHp),
    maxHealth(maxHp),
    advanceSpeed(30.0f),
    canDodge(true),
    isJumping(false),
    isDucking(false),
    actionTimer(0.0f),
    visualYOffset(0.0f),
    isFighting(false),
    attackCooldown(0.0f),
    attackInterval(0.65f) { // 0.65s cooldown prevents arrow spamming
}

void Player::jump() {
    if (!canDodge || isJumping || isDucking) {
        return;
    }
    isJumping = true;
    actionTimer = 0.45f;
    visualYOffset = -22.0f;
}

void Player::duck() {
    if (!canDodge || isJumping || isDucking) {
        return;
    }
    isDucking = true;
    actionTimer = 0.45f;
    visualYOffset = 22.0f;
}

void Player::update(float dt) {
    if (!active) return;

    // Halt advancement upon contact with an enemy; resume once the enemy is cleared
    if (!isFighting) {
        position.x += advanceSpeed * dt;
    }

    // Update dodge timers
    if (isJumping || isDucking) {
        actionTimer -= dt;
        if (actionTimer <= 0.0f) {
            isJumping = false;
            isDucking = false;
            visualYOffset = 0.0f;
        }
    }

    // Update attack cooldown
    if (attackCooldown > 0.0f) {
        attackCooldown -= dt;
    }
}

glm::vec2 Player::getRenderPosition() const {
    return glm::vec2(position.x, position.y + visualYOffset);
}

void Player::deploy(float startX, float startY, int newLane) {
    position = glm::vec2(startX, startY);
    laneId = newLane;
    health = maxHealth;
    active = true;
    isJumping = false;
    isDucking = false;
    actionTimer = 0.0f;
    visualYOffset = 0.0f;
    isFighting = false;
    attackCooldown = 0.0f;
}