#include "../headers/Player.h"
#include <cmath>

Player::Player(int maxHp, int lane)
    : Entity(maxHp, lane), health(maxHp), maxHealth(maxHp), advanceSpeed(30.0f),
      canDodge(true), isJumping(false), isDucking(false), actionTimer(0.0f),
      visualYOffset(0.0f), isFighting(false), attackCooldown(0.0f),
      attackInterval(0.65f), isAttacking(false), attackTimer(0.0f),
      rotation(0.0f) {
  // koll el defaults metzabbata fel initializer list
}

void Player::jump() {
  // law by-duck aw already by-not, faks
  if (!canDodge || isJumping || isDucking)
    return;

  isJumping = true;
  actionTimer = 0.45f;
  visualYOffset = -22.0f; // yetla3 le fo2
}

void Player::duck() {
  // nenzel ta7t 3ashan netfada el projectiles
  if (!canDodge || isJumping || isDucking)
    return;

  isDucking = true;
  actionTimer = 0.45f;
  visualYOffset = 22.0f; // yenzel le ta7t
}

void Player::update(float dt) {
  if (!active)
    return;

  // flash timer law etdarab
  if (damageFlashTimer > 0.0f) {
    damageFlashTimer -= dt;
  }

  // yemshe 3alatool law mafeesh 5ena2a
  if (!isFighting) {
    position.x += advanceSpeed * dt;
  }

  // timer el nott wel ducking
  if (isJumping || isDucking) {
    actionTimer -= dt;
    if (actionTimer <= 0.0f) {
      isJumping = false;
      isDucking = false;
      visualYOffset = 0.0f;
    }
  }

  // cooldown el dharb
  if (attackCooldown > 0.0f) {
    attackCooldown -= dt;
  }

  // animation el swing bta3et el attack
  if (isAttacking) {
    attackTimer += dt;

    // sine wave 3ashan ylef w yerga3 makanoh
    float swingRatio = attackTimer / 0.2f;
    rotation = std::sin(swingRatio * 3.14159265f) * 40.0f;

    if (attackTimer >= 0.2f) {
      isAttacking = false;
      attackTimer = 0.0f;
      rotation = 0.0f;
    }
  }
}

glm::vec2 Player::getRenderPosition() const {
  // position el rasm ma3 el offset bta3 el nott aw el duck
  return glm::vec2(position.x, position.y + visualYOffset);
}

void Player::deploy(float startX, float startY, int newLane) {
  // reset kamel lil unit lama ne-nazelha el field
  position = glm::vec2(startX, startY);
  laneId = newLane;
  health = maxHealth;
  damageFlashTimer = 0.0f;
  active = true;

  // zeroing out el actions wel status
  isJumping = false;
  isDucking = false;
  actionTimer = 0.0f;
  visualYOffset = 0.0f;
  isFighting = false;
  attackCooldown = 0.0f;
  isAttacking = false;
  attackTimer = 0.0f;
  rotation = 0.0f;
}