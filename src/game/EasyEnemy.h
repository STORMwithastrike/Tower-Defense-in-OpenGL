#pragma once

#include "Enemy.h"

// Easy Enemy: Upper Ranged Attacking Knight / Arbalist
// Starts with 5 lives. Deals 1 damage. Attacks from a distance using attacks
// that come from above (duck to avoid).
class EasyEnemy : public Enemy {
public:
  EasyEnemy(int lane, const glm::vec2 &startPos);

  void Update(float dt,
              std::vector<EnemyProjectile> &activeProjectiles) override;
  void Render(Renderer &renderer) override;

private:
  float reloadAnim;
};
