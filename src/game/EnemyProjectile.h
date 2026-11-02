#pragma once

#include <glm/glm.hpp>
#include "Types.h"
#include "../render/Renderer.h"

class EnemyProjectile {
public:
    EnemyProjectile(int lane, const glm::vec2& pos, float speed, AttackHeight height, int damage = 1);

    void Update(float dt);
    void Render(Renderer& renderer);

    bool IsActive() const { return active; }
    void Deactivate() { active = false; }

    int GetLane() const { return lane; }
    AttackHeight GetAttackHeight() const { return attackHeight; }
    int GetDamage() const { return damage; }
    BoundingBox GetBounds() const;
    const glm::vec2& GetPosition() const { return position; }

private:
    int lane;
    glm::vec2 position;
    float speed;
    AttackHeight attackHeight;
    int damage;
    bool active;
    float animTimer;
};

