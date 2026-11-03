#include "CollisionSystem.h"
#include "../../headers/DefensePlayer.h"
#include <algorithm>

CollisionSystem::CollisionSystem() {}

bool CollisionSystem::CheckHomeBaseBreach(const std::vector<std::unique_ptr<Enemy>>& enemies, HomeBase& homeBase) {
    float baseX = homeBase.getPosition().x + homeBase.getSize().x;

    for (const auto& enemy : enemies) {
        if (!enemy || enemy->IsDead()) continue;
        if (enemy->HasReachedBase(baseX)) {
            // Enemies reaching the castle cause direct structural damage
            homeBase.takeDamage(enemy->GetDamage());
            return homeBase.hasLostGame();
        }
    }
    return homeBase.hasLostGame();
}

Player *CollisionSystem::findShieldingDefensePlayer(
    const std::vector<Player *> &players, int lane, float targetX) {
  Player *bestShield = nullptr;
  float bestX = 999999.0f;

  for (Player *p : players) {
    if (!p || !p->isAlive() || p->getLaneId() != lane)
      continue;
    // DefensePlayer is stationary (advanceSpeed == 0.0f)
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

static BoundingBox getPlayerBounds(const Player *p) {
  glm::vec2 pos = p->getRenderPosition();
  float h = p->getIsDucking() ? 24.0f : 44.0f;
  return BoundingBox(pos.x - 14.0f, pos.y - 20.0f, 28.0f, h);
}

void CollisionSystem::CheckPlayerEnemyEncounters(std::vector<Player*>& players,
                                                 std::vector<std::unique_ptr<Enemy>>& enemies)
{
    for (Player* p : players) {
        if (!p || !p->isAlive()) continue;
        BoundingBox pBox = getPlayerBounds(p);

    for (auto &e : enemies) {
      if (!e || e->IsDead())
        continue;
      if (p->getLaneId() != e->GetLane())
        continue;

      BoundingBox eBox = e->GetBounds();

      if (pBox.Intersects(eBox) ||
          (pBox.x + pBox.width >= eBox.x && pBox.x <= eBox.x + eBox.width)) {
        e->SetFighting(true);
      }
    }
  }
}

void CollisionSystem::CheckPlayerCollectiblePickups(
    std::vector<Player *> &players, std::vector<Collectible> &collectibles,
    int &totalCoins) {
  for (auto &col : collectibles) {
    if (!col.IsActive())
      continue;
    BoundingBox cBox = col.GetBounds();

    for (Player *p : players) {
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
    std::vector<Projectile> &arrows,
    std::vector<std::unique_ptr<Enemy>> &enemies) {
  for (auto &arrow : arrows) {
    if (!arrow.isActive())
      continue;
    glm::vec2 pos = arrow.getPosition();
    BoundingBox arrowBox(pos.x - 10.0f, pos.y - 5.0f, 20.0f, 10.0f);

    for (auto &e : enemies) {
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
    for (Player *p : players) {
      if (!p || !p->isAlive() || p->getLaneId() != lane)
        continue;

      if (projBox.Intersects(getPlayerBounds(p))) {
        // 1. Check Defense Player shielding
        Player *shield =
            findShieldingDefensePlayer(players, lane, p->getPosition().x);
        Player *target = (shield && shield != p) ? shield : p;

                // 2. Check Dodging Mechanics
                if (proj.GetAttackHeight() == AttackHeight::LOW && target->getIsJumping()) {
                    continue; // Dodged low attack!
                }
                if (proj.GetAttackHeight() == AttackHeight::HIGH && target->getIsDucking()) {
                    continue; // Dodged high attack!
                }

                target->takeDamage(proj.GetDamage());
                proj.Deactivate();
                hitPlayer = true;
                break;
            }
        }

        // 3. Castle Collision: If projectile passes players and reaches the castle, it damages the castle!
        if (!hitPlayer && proj.IsActive()) {
            if (proj.GetPosition().x <= castleEdgeX) {
                homeBase.takeDamage(proj.GetDamage());
                proj.Deactivate();
            }
        }
    }
}
