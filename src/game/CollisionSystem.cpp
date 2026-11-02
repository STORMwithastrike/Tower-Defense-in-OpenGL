#include "CollisionSystem.h"
#include <algorithm>

CollisionSystem::CollisionSystem() {}

bool CollisionSystem::CheckHomeBaseBreach(const std::vector<std::unique_ptr<Enemy>>& enemies, float homeBaseMaxX) {
    for (const auto& enemy : enemies) {
        if (!enemy || enemy->IsDead()) continue;
        if (enemy->HasReachedBase(homeBaseMaxX)) {
            return true; // Breach! Instant loss condition
        }
    }
    return false;
}

Player* CollisionSystem::findShieldingDefensePlayer(const std::vector<std::unique_ptr<Player>>& players,
                                                    int lane, float targetX)
{
    // Find a stationary Defense Player in front of targetX on the same lane
    Player* bestShield = nullptr;
    float bestX = 999999.0f;

    for (const auto& p : players) {
        if (!p || p->IsDead() || p->GetLane() != lane) continue;
        if (p->GetType() == PlayerType::DEFENSE) {
            float shieldX = p->GetPosition().x;
            // Shield must be between target and the incoming attack (shieldX >= targetX)
            if (shieldX >= targetX && shieldX < bestX) {
                bestX = shieldX;
                bestShield = p.get();
            }
        }
    }
    return bestShield;
}

void CollisionSystem::CheckPlayerEnemyEncounters(std::vector<std::unique_ptr<Player>>& players,
                                                 std::vector<std::unique_ptr<Enemy>>& enemies)
{
    // Reset fighting states initially
    for (auto& p : players) {
        if (p && !p->IsDead()) p->SetFighting(false);
    }
    for (auto& e : enemies) {
        if (e && !e->IsDead()) e->SetFighting(false);
    }

    // Check collisions on identical lanes
    for (auto& p : players) {
        if (!p || p->IsDead()) continue;
        BoundingBox pBox = p->GetBounds();

        for (auto& e : enemies) {
            if (!e || e->IsDead()) continue;
            if (p->GetLane() != e->GetLane()) continue;

            BoundingBox eBox = e->GetBounds();

            // When player and enemy meet/collide on the lane:
            // Both stop moving and fight until one is defeated
            if (pBox.Intersects(eBox) || (pBox.x + pBox.width >= eBox.x && pBox.x <= eBox.x + eBox.width)) {
                p->SetFighting(true);
                e->SetFighting(true);
            }
        }
    }
}

void CollisionSystem::CheckPlayerCollectiblePickups(std::vector<std::unique_ptr<Player>>& players,
                                                    std::vector<Collectible>& collectibles,
                                                    int& totalCoins)
{
    for (auto& col : collectibles) {
        if (!col.IsActive()) continue;
        BoundingBox cBox = col.GetBounds();

        for (auto& p : players) {
            if (!p || p->IsDead() || p->GetLane() != col.GetLane()) continue;

            if (p->GetBounds().Intersects(cBox)) {
                totalCoins += col.GetValue();
                col.Collect();
                break;
            }
        }
    }
}

void CollisionSystem::CheckProjectileCollisions(std::vector<Projectile>& projectiles,
                                                std::vector<std::unique_ptr<Player>>& players,
                                                std::vector<std::unique_ptr<Enemy>>& enemies)
{
    for (auto& proj : projectiles) {
        if (!proj.IsActive()) continue;
        BoundingBox projBox = proj.GetBounds();
        int lane = proj.GetLane();

        if (proj.IsFriendly()) {
            // Friendly projectile hits the foremost enemy in the lane
            for (auto& e : enemies) {
                if (!e || e->IsDead() || e->GetLane() != lane) continue;

                if (projBox.Intersects(e->GetBounds())) {
                    e->TakeDamage(proj.GetDamage());
                    proj.Deactivate();
                    break;
                }
            }
        } else {
            // Hostile enemy projectile moving towards players
            // Check targets in lane
            for (auto& p : players) {
                if (!p || p->IsDead() || p->GetLane() != lane) continue;

                if (projBox.Intersects(p->GetBounds())) {
                    // 1. Check Defense Player shielding rule:
                    // If a player is behind a Defense Player on the same lane, Defense Player absorbs the hit!
                    Player* shield = findShieldingDefensePlayer(players, lane, p->GetPosition().x);
                    Player* target = (shield && shield != p.get()) ? shield : p.get();

                    // 2. Check Dodging Mechanics:
                    // Jumping avoids low attacks
                    if (proj.GetAttackHeight() == AttackHeight::LOW && target->IsJumping()) {
                        // Successfully avoided low attack!
                        continue;
                    }

                    // Ducking avoids high attacks
                    if (proj.GetAttackHeight() == AttackHeight::HIGH && target->IsDucking()) {
                        // Successfully avoided high attack!
                        continue;
                    }

                    // Hit connects!
                    target->TakeDamage(proj.GetDamage());
                    proj.Deactivate();
                    break;
                }
            }
        }
    }
}

