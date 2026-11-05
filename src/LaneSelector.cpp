#include "../headers/LaneSelector.h"

LaneSelector::LaneSelector()
    : hoveredLaneId(-1), selectedLaneId(-1), wasMouseClicked(false) {
}

void LaneSelector::update(const Grid& grid, double mouseX, double mouseY, bool isClicking) {
    hoveredLaneId = -1;

    for (const auto& lane : grid.getLanes()) {
        if (mouseY >= lane.minY && mouseY <= lane.maxY &&
            mouseX >= lane.minX && mouseX <= lane.maxX) {
            hoveredLaneId = lane.id;

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

int LaneSelector::getHoveredLaneId() const {
    return hoveredLaneId;
}

int LaneSelector::getSelectedLaneId() const {
    return selectedLaneId;
}