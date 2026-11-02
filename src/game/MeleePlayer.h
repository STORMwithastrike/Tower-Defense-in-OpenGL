#pragma once

#include "Player.h"

// Melee Player: Friendly Blue Knight of the Royal Guard
// Advances towards the right automatically; jumps and ducks on command.
// Wields close-range sword that animates in hand; deals 1 damage. 5 lives.
class MeleePlayer : public Player {
public:
    MeleePlayer(int lane, const glm::vec2& startPos);

    void Update(float dt) override;
    void Render(Renderer& renderer) override;

private:
    float swingTimer;
};

