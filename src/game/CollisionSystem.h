#pragma once

#include <vector>
#include <memory>
#include "Types.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Collectible.h"

class CollisionSystem {
public:
    CollisionSystem();

    // Checks if any enemy has reached or breached the home base (leftmost edge)
    bool CheckHomeBaseBreach(const std::vector<std::unique_ptr<Enemy>>& enemies, float homeBaseMaxX);

    // Updates physical combat engagement between players and enemies on the same lane
    void CheckPlayerEnemyEncounters(std::vector<std::unique_ptr<Player>>& players,
                                    std::vector<std::unique_ptr<Enemy>>& enemies);

    // Checks player collisions with dropped collectibles
    void CheckPlayerCollectiblePickups(std::vector<std::unique_ptr<Player>>& players,
                                       std::vector<Collectible>& collectibles,
                                       int& totalCoins);

    // Checks projectile hits against targets with dodging & defense shielding rules
    void CheckProjectileCollisions(std::vector<Projectile>& projectiles,
                                   std::vector<std::unique_ptr<Player>>& players,
                                   std::vector<std::unique_ptr<Enemy>>& enemies);

private:
    // Helper to find the frontmost living Defense Player on a lane that can shield allies behind it
    Player* findShieldingDefensePlayer(const std::vector<std::unique_ptr<Player>>& players,
                                       int lane, float targetX);
};

