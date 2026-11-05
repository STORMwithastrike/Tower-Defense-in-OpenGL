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
private:
    CardUnitType unitType;
    int cost;
    glm::vec2 position;
    glm::vec2 size;

    bool affordable;
    bool hovered;
    bool selected;
    bool wasMousePressed;

    bool contains(double x, double y) const;
    void renderMiniModel(Renderer& renderer) const;

public:
    PlayerCardUI(CardUnitType unitType, int cost, const glm::vec2& position, const glm::vec2& size);

    void update(const Economy& economy, double mouseX, double mouseY);
    void render(Renderer& renderer) const;
    bool handleMouseClick(double mouseX, double mouseY, bool isPressed);

    CardUnitType getUnitType() const { return unitType; }
    int getCost() const { return cost; }
    bool isAffordable() const { return affordable; }
    bool isSelected() const { return selected; }
    void setSelected(bool s) { selected = s; }
    const glm::vec2& getPosition() const { return position; }
    const glm::vec2& getSize() const { return size; }
};