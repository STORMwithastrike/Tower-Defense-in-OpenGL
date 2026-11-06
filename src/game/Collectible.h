#pragma once

#include <glm/glm.hpp>
#include "Types.h"
#include "../render/Renderer.h"

// el coin aw el drop elly byo2a3 fl lane
class Collectible {
private:
    int lane;           // anhy lane
    glm::vec2 position; // makanha
    int value;          // el gold bta3ha
    bool active;        // mawgooda wala et-lammet
    float animTimer;    // timer lel floating animation

public:
    Collectible(int lane, const glm::vec2& pos, int value = 5);

    // update timer el animation
    void Update(float dt);

    // rasm el coin
    void Render(Renderer& renderer);

    bool IsActive() const { return active; }
    void Collect() { active = false; }

    int GetLane() const { return lane; }
    int GetValue() const { return value; }

    // bounding box 3ashan el click
    BoundingBox GetBounds() const;
    const glm::vec2& GetPosition() const { return position; }
};