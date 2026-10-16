#include "../headers/LaneSelector.h"
#include <iostream>

LaneSelector::LaneSelector()
	: m_hoveredLaneId(-1), m_selectedLaneId(-1), m_wasMouseClicked(false) {

}

void LaneSelector::update(const Grid& grid, double mouseX, double mouseY, bool isClicking) {
	m_hoveredLaneId = -1;

	for (const auto& lane : grid.getLanes()) {

		if (mouseY >= lane.minY && mouseY <= lane.maxY &&
			mouseX >= lane.minX && mouseY <= lane.maxX) {

			m_hoveredLaneId = lane.id;

			if (isClicking && !m_wasMouseClicked) {
				m_selectedLaneId = lane.id;
				std::cout << "Lane" << lane.id << " Selected! Center Y: " << lane.centerY << "\n";
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