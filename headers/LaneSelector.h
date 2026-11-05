#pragma once

#include "Grid.h"

class LaneSelector {
private:
    int hoveredLaneId;
    int selectedLaneId;
    bool wasMouseClicked;

public:
    LaneSelector();

    void update(const Grid& grid, double mouseX, double mouseY, bool isClicking);

    int getHoveredLaneId() const;
    int getSelectedLaneId() const;
};