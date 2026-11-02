#pragma once
#include "Grid.h"

class LaneSelector {
private:
	int m_hoveredLaneId;
	int m_selectedLaneId;
	bool m_wasMouseClicked;

public:
	LaneSelector();

	void update(const Grid& grid, double mouseX, double mouseY, bool isClicking);

	int getHoveredLaneId() const;
	int getSelectedLaneId() const;

};