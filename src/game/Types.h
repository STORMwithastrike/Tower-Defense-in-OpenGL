#pragma once

#include <glm/glm.hpp>
#include <algorithm>
#include "../../headers/WaveManager.h"

struct BoundingBox {
    float x;
    float y;
    float width;
    float height;

    BoundingBox() : x(0.0f), y(0.0f), width(0.0f), height(0.0f) {}
    BoundingBox(float x, float y, float w, float h) : x(x), y(y), width(w), height(h) {}

    bool Intersects(const BoundingBox& other) const {
        return (x < other.x + other.width &&
                x + width > other.x &&
                y < other.y + other.height &&
                y + height > other.y);
    }

    glm::vec2 Center() const {
        return glm::vec2(x + width * 0.5f, y + height * 0.5f);
    }
};

enum class AttackHeight {
    NONE = 0,
    LOW,    // Ground level: Player must JUMP (W/Up) to avoid
    HIGH,   // Upper level: Player must DUCK (S/Down) to avoid
    MELEE   // Close-range encounter
};

enum class PlayerType {
    MELEE,
    RANGED,
    DEFENSE
};
