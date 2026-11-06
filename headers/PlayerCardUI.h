#pragma once

#include <glm/glm.hpp>
#include "../headers/Economy.h"
#include "render/Renderer.h"

// anwa3 el abtal elly momken neshtreeha
enum class CardUnitType {
    Melee,
    Ranged,
    Defense
};

// UI card fl HUD ta7t bneshtery menha el abtal
class PlayerCardUI {
private:
    CardUnitType unitType;  // noo3 el hero
    int cost;               // se3ro bel gold
    glm::vec2 position;     // makan el card
    glm::vec2 size;         // 7agm el card

    bool affordable;        // ma3ana flous tekfy wala la2
    bool hovered;           // el mouse wa2ef fo2aha
    bool selected;          // me5tareen el card delwa2ty
    bool wasMousePressed;   // 3ashan single click detection

    // check law el mouse gowa 7dood el card
    bool contains(double x, double y) const;

    // btersom preview so3'ayar lel hero gowa el card
    void renderMiniModel(Renderer& renderer) const;

public:
    PlayerCardUI(CardUnitType unitType, int cost, const glm::vec2& position, const glm::vec2& size);

    // bt-check el economy wel hover kol frame
    void update(const Economy& economy, double mouseX, double mouseY);

    // rasm el card wel icon wel se3r
    void render(Renderer& renderer) const;

    // bt-handle clicket el mouse 3ashan el selection
    bool handleMouseClick(double mouseX, double mouseY, bool isPressed);

    // getters w setters inline 3ashan el main loop
    CardUnitType getUnitType() const { return unitType; }
    int getCost() const { return cost; }
    bool isAffordable() const { return affordable; }
    bool isSelected() const { return selected; }
    void setSelected(bool s) { selected = s; }
    const glm::vec2& getPosition() const { return position; }
    const glm::vec2& getSize() const { return size; }
};