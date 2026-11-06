#include "../headers/LaneSelector.h"

// bn-set el default values b -1 (mafeesh lane hovered wala selected)
LaneSelector::LaneSelector()
    : hoveredLaneId(-1), selectedLaneId(-1), wasMouseClicked(false) {
}

void LaneSelector::update(const Grid& grid, double mouseX, double mouseY, bool isClicking) {
    hoveredLaneId = -1;

    // bn-loop 3ala kol lane nshof el mouse gowa 7doodo wala la2
    for (const auto& lane : grid.getLanes()) {
        if (mouseY >= lane.minY && mouseY <= lane.maxY &&
            mouseX >= lane.minX && mouseX <= lane.maxX) {
            hoveredLaneId = lane.id;

            // edge detection lel click 3ashan n-toggle el select
            if (isClicking && !wasMouseClicked) {
                if (selectedLaneId == lane.id) {
                    selectedLaneId = -1;
                }
                else {
                    selectedLaneId = lane.id;
                }
            }
            break;
        }
    }
    wasMouseClicked = isClicking;
}

// getters 3ashan el rasm wel logic y3rafo anhy lane sha3'aleen 3aleeh
int LaneSelector::getHoveredLaneId() const {
    return hoveredLaneId;
}

int LaneSelector::getSelectedLaneId() const {
    return selectedLaneId;
}