#pragma once

#include <vector>
#include <memory>
#include "Types.h"
#include "Player.h"
#include "Enemy.h"
#include "Collectible.h"

class CombatSystem {
public:
    CombatSystem();

    // Updates active melee fights, damage calculations, and drops
    void Update(float dt,
                std::vector<std::unique_ptr<Player>>& players,
                std::vector<std::unique_ptr<Enemy>>& enemies,
                std::vector<Collectible>& collectibles);

    // Callbacks/counters for wave management
    int GetDefeatedEnemyCount() const { return defeatedCount; }
    void ResetDefeatedEnemyCount() { defeatedCount = 0; }

private:
    float combatTickTimer;
    float combatInterval; // Attack rate when clashing (1.0 sec)
    int defeatedCount;
};

