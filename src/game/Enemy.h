#pragma once

#include "../render/Renderer.h"
#include "EnemyProjectile.h"
#include "Types.h"
#include <glm/glm.hpp>
#include <vector>

class Enemy {
public:
  Enemy(int lane, const glm::vec2 &startPos, EnemyType type, int maxHealth,
        int damage, float speed);
  virtual ~Enemy() = default;

  virtual void Update(float dt,
                      std::vector<EnemyProjectile> &activeProjectiles);
  virtual void Render(Renderer &renderer) = 0;

  virtual void TakeDamage(int damage);
  bool IsDead() const { return health <= 0; }

  int GetLane() const { return lane; }
  EnemyType GetType() const { return type; }
  const glm::vec2 &GetPosition() const { return position; }
  void SetPosition(const glm::vec2 &pos) { position = pos; }

  int GetHealth() const { return health; }
  int GetMaxHealth() const { return maxHealth; }
  int GetDamage() const { return damage; }

  bool IsFighting() const { return fighting; }
  void SetFighting(bool f) { fighting = f; }

  bool HasReachedBase(float homeBaseMaxX) const {
    return position.x <= homeBaseMaxX;
  }

  virtual BoundingBox GetBounds() const;

  // Red tint intensity based on missing health + hit flash
  float GetDamageIntensity() const;

protected:
  int lane;
  glm::vec2 position;
  float speed;
  EnemyType type;
  int health;
  int maxHealth;
  int damage;

  bool fighting;
  float attackTimer;
  float attackCooldown;

  float damageFlashTimer;
  float walkAnimTimer;

  // Ranged attack timer
  float shootTimer;
  float shootInterval;

  void renderHealthBar(Renderer &renderer, float yOffset = -34.0f) const;
};
