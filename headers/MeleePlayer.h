#pragma once

#include "Player.h"

class MeleePlayer : public Player {
private:
    void generateMesh();

public:
    MeleePlayer(int lane = 0);
    void init(float startX, float startY);
};