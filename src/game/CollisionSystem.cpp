#include "CollisionSystem.h"
#include "../../headers/DefensePlayer.h"
#include <algorithm>

CollisionSystem::CollisionSystem() {}

bool CollisionSystem::CheckHomeBaseBreach(const std::vector<std::unique_ptr<Enemy>>& enemies, HomeBase& homeBase) {
    float baseX = homeBase.getPosition().x + homeBase.getSize().x;

    for (const auto& enemy : enemies) {
        if (!enemy || enemy->IsDead()) continue;
        if (enemy->HasReachedBase(baseX)) {
            homeBase.takeDamage(enemy->GetDamage());
            return homeBase.hasLostGame();
        }
    }
    return homeBase.hasLostGame();
}

Player* CollisionSystem::findShieldingDefensePlayer(
    const std::vector<Player*>& players, int lane, float targetX) {
    Player* bestShield = nullptr;
    float bestX = 999999.0f;

    for (Player* p : players) {
        if (!p || !p->isAlive() || p->getLaneId() != lane)
            continue;
        if (p->getAdvanceSpeed() == 0.0f) {
            float shieldX = p->getPosition().x;
            if (shieldX >= targetX && shieldX < bestX) {
                bestX = shieldX;
                bestShield = p;
            }
        }
    }
    return bestShield;
}

static BoundingBox getPlayerBounds(const Player* p) {
    glm::vec2 pos = p->getRenderPosition();
    float h = p->getIsDucking() ? 20.0f : (p->getIsJumping() ? 32.0f : 44.0f);
    float yOffset = p->getIsDucking() ? -10.0f : -20.0f;
    return BoundingBox(pos.x - 12.0f, pos.y + yOffset, 24.0f, h);
}

void CollisionSystem::CheckPlayerEnemyEncounters(std::vector<Player*>& players,
    std::vector<std::unique_ptr<Enemy>>& enemies)
{
    // Reset fighting state on all players at frame start
    for (Player* p : players) {
        if (p) p->setFighting(false);
    }

    for (auto& e : enemies) {
        if (!e || e->IsDead())
            continue;

        bool enemyFighting = false;

        for (Player* p : players) {
            if (!p || !p->isAlive()) continue;
            if (p->getLaneId() != e->GetLane()) continue;

            // Directional check: Enemy must be in front of the defender (to the right)
            if (e->GetPosition().x < p->getPosition().x - 15.0f) {
                continue;
            }

            BoundingBox pBox = getPlayerBounds(p);
            BoundingBox eBox = e->GetBounds();

            // When bounds touch, halt both the enemy and the player
            if (pBox.Intersects(eBox) ||
                (pBox.x + pBox.width >= eBox.x && pBox.x <= eBox.x + eBox.width)) {
                enemyFighting = true;
                p->setFighting(true); // Melee, Ranged, and Defense all stop moving
            }
        }

        e->SetFighting(enemyFighting);
    }
}

void CollisionSystem::CheckPlayerCollectiblePickups(
    std::vector<Player*>& players, std::vector<Collectible>& collectibles,
    int& totalCoins)
{
    for (auto& col : collectibles) {
        if (!col.IsActive())
            continue;
        BoundingBox cBox = col.GetBounds();

        for (Player* p : players) {
            if (!p || !p->isAlive() || p->getLaneId() != col.GetLane())
                continue;

            if (getPlayerBounds(p).Intersects(cBox)) {
                totalCoins += col.GetValue();
                col.Collect();
                break;
            }
        }
    }
}

void CollisionSystem::CheckPlayerArrows(
    std::vector<Projectile>& arrows,
    std::vector<std::unique_ptr<Enemy>>& enemies)
{
    for (auto& arrow : arrows) {
        if (!arrow.isActive())
            continue;
        glm::vec2 pos = arrow.getPosition();
        BoundingBox arrowBox(pos.x - 10.0f, pos.y - 5.0f, 20.0f, 10.0f);

        for (auto& e : enemies) {
            if (!e || e->IsDead())
                continue;

            if (arrowBox.Intersects(e->GetBounds())) {
                e->TakeDamage(arrow.getDamage());
                arrow.setActive(false);
                break;
            }
        }
    }
}

void CollisionSystem::CheckEnemyProjectiles(std::vector<EnemyProjectile>& enemyProjectiles,
    std::vector<Player*>& players,
    HomeBase& homeBase)
{
    float castleEdgeX = homeBase.getPosition().x + homeBase.getSize().x;

    for (auto& proj : enemyProjectiles) {
        if (!proj.IsActive()) continue;
        BoundingBox projBox = proj.GetBounds();
        int lane = proj.GetLane();

        bool hitPlayer = false;
        for (Player* p : players) {
            if (!p || !p->isAlive() || p->getLaneId() != lane)
                continue;

            // Dodge check: jumping avoids low projectile, ducking avoids high projectile
            if (proj.GetAttackHeight() == AttackHeight::LOW && p->getIsJumping()) {
                continue;
            }
            if (proj.GetAttackHeight() == AttackHeight::HIGH && p->getIsDucking()) {
                continue;
            }

            if (projBox.Intersects(getPlayerBounds(p))) {
                Player* shield = findShieldingDefensePlayer(players, lane, p->getPosition().x);
                Player* target = (shield && shield != p) ? shield : p;

                if (proj.GetAttackHeight() == AttackHeight::LOW && target->getIsJumping()) {
                    continue;
                }
                if (proj.GetAttackHeight() == AttackHeight::HIGH && target->getIsDucking()) {
                    continue;
                }

                target->takeDamage(proj.GetDamage());
                proj.Deactivate();
                hitPlayer = true;
                break;
            }
        }

        if (!hitPlayer && proj.IsActive()) {
            if (proj.GetPosition().x <= castleEdgeX) {
                homeBase.takeDamage(proj.GetDamage());
                proj.Deactivate();
            }
        }
    }
}