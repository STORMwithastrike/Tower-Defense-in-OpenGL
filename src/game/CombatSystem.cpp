#include "CombatSystem.h"
#include "../../headers/DefensePlayer.h"
#include <algorithm>

CombatSystem::CombatSystem()
    : combatTickTimer(0.0f), combatInterval(0.95f), defeatedCount(0) {}

void CombatSystem::Update(float dt, std::vector<Player *> &players,
                          std::vector<std::unique_ptr<Enemy>> &enemies,
                          std::vector<Collectible> &collectibles) {
  combatTickTimer += dt;
  bool shouldTickDamage = (combatTickTimer >= combatInterval);
  if (shouldTickDamage) {
    combatTickTimer = 0.0f;
  }

  // 1. Process active melee combat encounters
  for (Player *p : players) {
    if (!p || !p->isAlive())
      continue;

    for (auto &e : enemies) {
      if (!e || e->IsDead() || !e->IsFighting())
        continue;
      if (p->getLaneId() != e->GetLane())
        continue;

      if (shouldTickDamage) {
        // Friendly attack (all except stationary defense deal 1 damage)
        if (p->getAdvanceSpeed() > 0.0f) {
          e->TakeDamage(1);
        }

        // Enemy attack (1 damage or 2 for Hard Melee)
        p->takeDamage(e->GetDamage());
      }
    }
  }

  // 2. Process Enemy Deaths & Drops
  for (auto it = enemies.begin(); it != enemies.end();) {
    if ((*it)->IsDead()) {
      collectibles.emplace_back((*it)->GetLane(), (*it)->GetPosition(), 10);
      defeatedCount++;
      it = enemies.erase(it);
    } else {
      ++it;
    }
  }
}
