#pragma once

#include "Entity.h"

class Player : public Entity {
protected:
    float advanceSpeed;
    bool canDodge;

    bool isJumping;
    bool isDucking;
    float actionTimer;
    float visualYOffset;

public:
    Player(int maxHp = 5, int lane = 0);

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