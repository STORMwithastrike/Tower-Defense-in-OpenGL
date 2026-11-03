#pragma once

#include "../../headers/Player.h"
#include "../../headers/Projectile.h"
#include "../../headers/HomeBase.h"
#include "Collectible.h"
#include "Enemy.h"
#include "EnemyProjectile.h"
#include "Types.h"
#include <memory>
#include <vector>

class CollisionSystem {
public:
    CollisionSystem();

    // Checks if any enemy has reached the home base
    bool CheckHomeBaseBreach(const std::vector<std::unique_ptr<Enemy>>& enemies, HomeBase& homeBase);

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
    // If projectiles go past the players, they collide with the castle home base and cause damage!
    void CheckEnemyProjectiles(std::vector<EnemyProjectile>& enemyProjectiles,
                               std::vector<Player*>& players,
                               HomeBase& homeBase);

private:
    Player* findShieldingDefensePlayer(const std::vector<Player*>& players, int lane, float targetX);
};
