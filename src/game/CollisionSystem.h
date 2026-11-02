#pragma once

#include <vector>
#include <memory>
#include "Types.h"
#include "../../headers/Player.h"
#include "../../headers/Projectile.h"
#include "Enemy.h"
#include "EnemyProjectile.h"
#include "Collectible.h"

class CollisionSystem {
public:
    CollisionSystem();

    // Checks if any enemy has reached or breached the home base (leftmost edge)
    bool CheckHomeBaseBreach(const std::vector<std::unique_ptr<Enemy>>& enemies, float homeBaseMaxX);

    // Updates physical combat engagement between players and enemies on the same lane
    void CheckPlayerEnemyEncounters(std::vector<Player*>& players,
                                    std::vector<std::unique_ptr<Enemy>>& enemies);

    // Checks player collisions with dropped collectibles
    void CheckPlayerCollectiblePickups(std::vector<Player*>& players,
                                       std::vector<Collectible>& collectibles,
                                       int& totalCoins);

    // Checks player arrow hits against enemies
    void CheckPlayerArrows(std::vector<Projectile>& arrows,
                           std::vector<std::unique_ptr<Enemy>>& enemies);

    // Checks hostile enemy projectile hits against players (with jumping/ducking and defense shielding)
    void CheckEnemyProjectiles(std::vector<EnemyProjectile>& enemyProjectiles,
                               std::vector<Player*>& players);

private:
    Player* findShieldingDefensePlayer(const std::vector<Player*>& players, int lane, float targetX);
};
