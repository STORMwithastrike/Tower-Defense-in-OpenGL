#pragma once

#include "Player.h"

class DefensePlayer : public Player {
private:
    void generateMesh();

public:
    DefensePlayer(int lane = 0);
    void init(float startX, float startY);
};