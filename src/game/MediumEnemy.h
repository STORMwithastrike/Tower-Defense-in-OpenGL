#pragma once

#include "Enemy.h"

// Medium Enemy: Lower Ranged Attacking Knight / Siege Sapper
// Starts with 5 lives. Deals 1 damage. Attacks from a distance using attacks that come from below (jump to avoid).
class MediumEnemy : public Enemy {
public:
    MediumEnemy(int lane, const glm::vec2& startPos);

    void Update(float dt, std::vector<EnemyProjectile>& activeProjectiles) override;
    void Render(Renderer& renderer) override;

private:
    float thrustAnim;
};
