#include "../headers/LaneSelector.h"
#include <iostream>

LaneSelector::LaneSelector()
	: m_hoveredLaneId(-1), m_selectedLaneId(-1), m_wasMouseClicked(false)
{
}

void LaneSelector::update(const Grid& grid, double mouseX, double mouseY, bool isClicking) {
	m_hoveredLaneId = -1;

	for (const auto& lane : grid.getLanes()) {
		// Checks full horizontal lane bounds
		if (mouseY >= lane.minY && mouseY <= lane.maxY &&
			mouseX >= lane.minX && mouseX <= lane.maxX)
		{
			m_hoveredLaneId = lane.id;

			if (isClicking && !m_wasMouseClicked) {
				// Toggle lane selection on click: clicking again deselects it
				if (m_selectedLaneId == lane.id) {
					m_selectedLaneId = -1;
				} else {
					m_selectedLaneId = lane.id;
				}
			}
			break;
		}
	}
	m_wasMouseClicked = isClicking;
}

int LaneSelector::getHoveredLaneId() const {
	return m_hoveredLaneId;
}

int LaneSelector::getSelectedLaneId() const {
	return m_selectedLaneId;
}
