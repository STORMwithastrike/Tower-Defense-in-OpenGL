#pragma once

#include <vector>
#include <memory>
#include "Types.h"
#include "../../headers/Player.h"
#include "Enemy.h"
#include "Collectible.h"

class CombatSystem {
public:
    CombatSystem();

    void Update(float dt,
                std::vector<Player*>& players,
                std::vector<std::unique_ptr<Enemy>>& enemies,
                std::vector<Collectible>& collectibles);

    int GetDefeatedEnemyCount() const { return defeatedCount; }
    void ResetDefeatedEnemyCount() { defeatedCount = 0; }

private:
    float combatTickTimer;
    float combatInterval;
    int defeatedCount;
};
