#include "CombatSystem.h"
#include "../../headers/DefensePlayer.h"
#include <algorithm>
#include <cmath>

CombatSystem::CombatSystem()
    : combatTickTimer(0.0f), combatInterval(0.95f), defeatedCount(0) {
}

void CombatSystem::Update(float dt, std::vector<Player*>& players,
    std::vector<std::unique_ptr<Enemy>>& enemies,
    std::vector<Collectible>& collectibles) {
    combatTickTimer += dt;
    bool shouldTickDamage = (combatTickTimer >= combatInterval);
    if (shouldTickDamage) {
        combatTickTimer = 0.0f;
    }

    // 1. Process active contact combat encounters for all player types
    for (Player* p : players) {
        if (!p || !p->isAlive())
            continue;

        for (auto& e : enemies) {
            if (!e || e->IsDead() || !e->IsFighting())
                continue;
            if (p->getLaneId() != e->GetLane())
                continue;

            // Contact check: Enemy and player must be directly in front of each other
            float dx = e->GetPosition().x - p->getPosition().x;
            if (dx < -10.0f || dx > 45.0f) {
                continue;
            }

            if (shouldTickDamage) {
                // DefensePlayer acts solely as a shield; Melee and Ranged deal 1 contact damage
                if (dynamic_cast<DefensePlayer*>(p) == nullptr) {
                    e->TakeDamage(1);
                }

                // Enemy attacks whichever player is blocking it
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
        }
        else {
            ++it;
        }
    }
}