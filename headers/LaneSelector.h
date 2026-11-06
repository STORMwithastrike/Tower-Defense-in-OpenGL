#pragma once

#include "Grid.h" // 3ashan neshof 7dood el lanes

// class mas2ool 3an el hover wel selection bta3 el lanes
class LaneSelector {
private:
    int hoveredLaneId;    // id el lane elly el mouse wa2ef fo2eeh (-1 lw mafeesh)
    int selectedLaneId;   // id el lane elly e5tarnah bel click
    bool wasMouseClicked; // 3ashan single click detection w my-spam-sh

public:
    LaneSelector();

    // bt-update el hover wel select 3ala 7asab makan el mouse
    void update(const Grid& grid, double mouseX, double mouseY, bool isClicking);

    // getters 3ashan el rasm wel game logic
    int getHoveredLaneId() const;
    int getSelectedLaneId() const;
};