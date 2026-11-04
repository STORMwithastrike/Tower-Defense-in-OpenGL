#pragma once

#include <glm/glm.hpp>
#include "../headers/Economy.h"
#include "render/Renderer.h"

enum class CardUnitType {
	Melee,
	Ranged,
	Defense
};

class PlayerCardUI {
public:
	PlayerCardUI(CardUnitType unitType, int cost, const glm::vec2& position, const glm::vec2& size);

	void update(const Economy& economy, double mouseX, double mouseY);

	void render(Renderer& renderer) const;

	bool handleMouseClick(double mouseX, double mouseY, bool isPressed);

	CardUnitType getUnitType() const { return m_unitType; }
	int getCost() const { return m_cost; }
	bool isAffordable() const { return m_isAffordable; }
	bool isSelected() const { return m_isSelected; }
	void setSelected(bool selected) { m_isSelected = selected; }
	const glm::vec2& getPosition() const { return m_position; }
	const glm::vec2& getSize() const { return m_size; }

private:
	CardUnitType m_unitType;
	int m_cost;
	glm::vec2 m_position;
	glm::vec2 m_size;

	bool m_isAffordable;
	bool m_isHovered;
	bool m_isSelected;
	bool m_wasMousePressed;

	bool contains(double x, double y) const;
	void renderMiniModel(Renderer& renderer) const;
};