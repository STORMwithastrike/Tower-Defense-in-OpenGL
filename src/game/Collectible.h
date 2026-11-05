#pragma once

#include <glm/glm.hpp>
#include "Types.h"
#include "../render/Renderer.h"

class Collectible {
private:
    int lane;
    glm::vec2 position;
    int value;
    bool active;
    float animTimer;

public:
    Collectible(int lane, const glm::vec2& pos, int value = 5);

    void Update(float dt);
    void Render(Renderer& renderer);

    bool IsActive() const { return active; }
    void Collect() { active = false; }

    int GetLane() const { return lane; }
    int GetValue() const { return value; }
    BoundingBox GetBounds() const;
    const glm::vec2& GetPosition() const { return position; }
};