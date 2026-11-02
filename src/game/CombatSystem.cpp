#include "CombatSystem.h"
#include <algorithm>

CombatSystem::CombatSystem()
    : combatTickTimer(0.0f),
      combatInterval(0.95f),
      defeatedCount(0)
{
}

void CombatSystem::Update(float dt,
                          std::vector<std::unique_ptr<Player>>& players,
                          std::vector<std::unique_ptr<Enemy>>& enemies,
                          std::vector<Collectible>& collectibles)
{
    combatTickTimer += dt;
    bool shouldTickDamage = (combatTickTimer >= combatInterval);
    if (shouldTickDamage) {
        combatTickTimer = 0.0f;
    }

    // 1. Process active melee combat engagements
    for (auto& p : players) {
        if (!p || p->IsDead() || !p->IsFighting()) continue;

        for (auto& e : enemies) {
            if (!e || e->IsDead() || !e->IsFighting()) continue;
            if (p->GetLane() != e->GetLane()) continue;

            if (shouldTickDamage) {
                // Friendly attack: All player types (except defense) deal 1 damage to enemies
                if (p->GetType() != PlayerType::DEFENSE) {
                    e->TakeDamage(1);
                }

                // Enemy attack: Deals 1 damage (or 2 damage for Hard Melee Enemy)
                p->TakeDamage(e->GetDamage());
            }
        }
    }

    // 2. Process Enemy Deaths & Drops
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if ((*it)->IsDead()) {
            // Defeated enemies drop collectibles as required by the specification
            collectibles.emplace_back((*it)->GetLane(), (*it)->GetPosition(), 10);
            defeatedCount++;
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }

    // 3. Process Player Deaths
    for (auto it = players.begin(); it != players.end(); ) {
        if ((*it)->IsDead()) {
            // Player defeated and removed from the scene
            it = players.erase(it);
        } else {
            ++it;
        }
    }
}

