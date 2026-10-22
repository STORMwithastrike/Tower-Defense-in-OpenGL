#pragma once

#include <vector>
#include "Player.h"

class ActionManager {
public:
    static void handleInput(std::vector<Player*>& players, const std::vector<int>& selectedLanes);
};