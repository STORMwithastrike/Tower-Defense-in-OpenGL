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

public:
    Player(int maxHp = 5, int lane = 0);

    void takeDamage(int amount) {
        health -= amount;
        if (health <= 0) {
            health = 0;
            active = false; // Defeated: removed from scene
        }
    }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    glm::vec4 getDamageTint() const {
        if (maxHealth <= 0) return glm::vec4(1.0f);
        float damageRatio = 1.0f - (static_cast<float>(health) / static_cast<float>(maxHealth));
        return glm::vec4(1.0f, 0.0f, 0.0f, damageRatio * 0.75f);
    }

    void jump();
    void duck();
    void update(float dt);

    // bygeb el position bta3 el jumping w ducking
    glm::vec2 getRenderPosition() const;

    bool getIsJumping() const { return isJumping; }
    bool getIsDucking() const { return isDucking; }
    float getAdvanceSpeed() const { return advanceSpeed; }
    void setAdvanceSpeed(float speed) { advanceSpeed = speed; }
    void setCanDodge(bool dodgeState) { canDodge = dodgeState; }
};