#pragma once
#include "Entity.h"
#include <glm/glm.hpp>

class Player : public Entity {
protected:
    int health;
    int maxHealth;

    float advanceSpeed;
    bool canDodge;

    bool isJumping;
    bool isDucking;
    float actionTimer;
    float visualYOffset;

    bool isFighting;
    float attackCooldown;
    float attackInterval;

    // Attack rotation animation
    bool isAttacking = false;
    float attackTimer = 0.0f;
    float rotation = 0.0f;
    float damageFlashTimer = 0.0f;

public:
    Player(int maxHp = 5, int lane = 0);
    virtual ~Player() = default;

    void takeDamage(int amount) {
        health -= amount;
        if (health <= 0) {
            health = 0;
            active = false;
        }
        damageFlashTimer = 0.35f;
    }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    float getDamageIntensity() const {
        if (maxHealth <= 0) return 0.0f;
        float healthDeficit = 1.0f - (static_cast<float>(health) / static_cast<float>(maxHealth));
        float baseRed = healthDeficit * 0.75f;
        if (damageFlashTimer > 0.0f) {
            baseRed = (baseRed > 0.90f) ? baseRed : 0.90f;
        }
        return (baseRed < 1.0f) ? baseRed : 1.0f;
    }
    glm::vec4 getDamageTint() const {
        float intensity = getDamageIntensity();
        return glm::vec4(1.0f, 0.0f, 0.0f, intensity);
    }

    void jump();
    void duck();
    void update(float dt);

    // Trigger attack animation and get current rotation
    void attack() {
        isAttacking = true;
        attackTimer = 0.0f;
    }
    float getRotation() const { return rotation; }

    glm::vec2 getRenderPosition() const;

    bool getIsJumping() const { return isJumping; }
    bool getIsDucking() const { return isDucking; }
    float getAdvanceSpeed() const { return advanceSpeed; }
    void setAdvanceSpeed(float speed) { advanceSpeed = speed; }
    void setCanDodge(bool dodgeState) { canDodge = dodgeState; }

    // Combat movement lock
    bool getIsFighting() const { return isFighting; }
    void setFighting(bool fighting) { isFighting = fighting; }

    // Ranged attack cooldown
    bool canAttack() const { return attackCooldown <= 0.0f; }
    void resetAttackCooldown() { attackCooldown = attackInterval; }
    void setAttackInterval(float interval) { attackInterval = interval; }

    void deploy(float startX, float startY, int newLane);
};