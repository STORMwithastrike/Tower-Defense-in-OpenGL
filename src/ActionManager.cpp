#include "../headers/ActionManager.h"
#include "../headers/InputManager.h"
#include <GLFW/glfw3.h>

void ActionManager::handleInput(std::vector<Player*>& players, const std::vector<int>& selectedLanes) {
    if (selectedLanes.empty()) {
        return;
    }

    // W or Up Arrow lel jump
    bool jumpPressed = InputManager::isKeyPressed(GLFW_KEY_W) || InputManager::isKeyPressed(GLFW_KEY_UP);

    // S or Down Arrow lel duck
    bool duckPressed = InputManager::isKeyPressed(GLFW_KEY_S) || InputManager::isKeyPressed(GLFW_KEY_DOWN);

    if (!jumpPressed && !duckPressed) {
        return;
    }

    for (Player* player : players) {
        if (!player || !player->isActive()) {
            continue;
        }

        // check lw player lane selcted
        for (int laneId : selectedLanes) {
            if (player->getLaneId() == laneId) {
                if (jumpPressed) {
                    player->jump(); 
                }
                else if (duckPressed) {
                    player->duck(); 
                }
                break;
            }
        }
    }
}