#include "../headers/Player.h"

Player::Player(int maxHp, int lane)
    : Entity(maxHp, lane),
    advanceSpeed(30.0f),
    canDodge(true),
    isJumping(false),
    isDucking(false),
    actionTimer(0.0f),
    visualYOffset(0.0f) {
}

void Player::jump() {
    // cant if
    if (!canDodge || isJumping || isDucking) {
        return;
    }
    isJumping = true;
    actionTimer = 0.45f;       
    visualYOffset = -22.0f;     // jump by shifting
}

void Player::duck() {
    // cant if
    if (!canDodge || isJumping || isDucking) {
        return;
    }
    isDucking = true;
    actionTimer = 0.45f;       
    visualYOffset = 22.0f;      // duck by shifiting 
}

void Player::update(float dt) {
    if (!active) return;

    // advance horizontally
    position.x += advanceSpeed * dt;

    // Update timers
    if (isJumping || isDucking) {
        actionTimer -= dt;
        if (actionTimer <= 0.0f) {
            isJumping = false;
            isDucking = false;
            visualYOffset = 0.0f;
        }
    }
}

glm::vec2 Player::getRenderPosition() const {
    return glm::vec2(position.x, position.y + visualYOffset);
}