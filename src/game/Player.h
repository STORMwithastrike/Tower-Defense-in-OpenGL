#pragma once

#include <glm/glm.hpp>
#include "Types.h"
#include "../render/Renderer.h"

class Player {
public:
    Player(int lane, const glm::vec2& startPos, PlayerType type, int maxHealth, float speed = 40.0f);
    virtual ~Player() = default;

    virtual void Update(float dt);
    virtual void Render(Renderer& renderer) = 0;

    virtual void Jump();
    virtual void Duck();

    virtual void TakeDamage(int damage);
    bool IsDead() const { return health <= 0; }

    int GetLane() const { return lane; }
    PlayerType GetType() const { return type; }
    const glm::vec2& GetPosition() const { return position; }
    void SetPosition(const glm::vec2& pos) { position = pos; }

    int GetHealth() const { return health; }
    int GetMaxHealth() const { return maxHealth; }

    bool IsJumping() const { return jumping; }
    bool IsDucking() const { return ducking; }
    bool IsStationary() const { return type == PlayerType::DEFENSE; }

    bool IsFighting() const { return fighting; }
    void SetFighting(bool f) { fighting = f; }

    virtual BoundingBox GetBounds() const;

protected:
    int lane;
    glm::vec2 position;
    float speed;
    PlayerType type;
    int health;
    int maxHealth;

    bool jumping;
    float jumpTimer;
    float jumpOffset;

    bool ducking;
    float duckTimer;

    bool fighting;
    float attackTimer;
    float attackCooldown;

    float damageFlashTimer;
    float walkAnimTimer;

    void updateDodging(float dt);
};

