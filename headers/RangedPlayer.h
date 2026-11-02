#pragma once

#include "Player.h"

class RangedPlayer : public Player {
private:
    void generateMesh();

public:
    RangedPlayer(int lane = 0);
    void init(float startX, float startY);
};