#pragma once

#include "Enemy.h"

// Hard Enemy: Melee Runner Attacking Knight / Dark Berserker
// Starts with 7 lives. Runs toward the player and attacks at close range.
// Deals 2 points of damage per attack!
class HardEnemy : public Enemy {
public:
    HardEnemy(int lane, const glm::vec2& startPos);

    void Update(float dt, std::vector<Projectile>& activeProjectiles) override;
    void Render(Renderer& renderer) override;

    // Melee attack animation trigger
    void TriggerSlash();

private:
    float swordAngle;
    float swingAnimTimer;
};

