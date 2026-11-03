#include "../headers/PlayerCardUI.h"

PlayerCardUI::PlayerCardUI(CardUnitType unitType, int cost, const glm::vec2& position, const glm::vec2& size)
    : m_unitType(unitType),
    m_cost(cost),
    m_position(position),
    m_size(size),
    m_isAffordable(false),
    m_isHovered(false),
    m_isSelected(false),
    m_wasMousePressed(false) {
}

bool PlayerCardUI::contains(double x, double y) const {
    return (x >= m_position.x && x <= m_position.x + m_size.x &&
        y >= m_position.y && y <= m_position.y + m_size.y);
}

void PlayerCardUI::update(const Economy& economy, double mouseX, double mouseY) {
    // 1. Check affordability against central Economy
    m_isAffordable = economy.canAfford(m_cost);

    // 2. Track mouse hover
    m_isHovered = contains(mouseX, mouseY);
}

bool PlayerCardUI::handleMouseClick(double mouseX, double mouseY, bool isPressed) {
    bool clicked = false;

    // Trigger on mouse down while hovered and affordable
    if (isPressed && !m_wasMousePressed && m_isHovered && m_isAffordable) {
        m_isSelected = !m_isSelected;
        clicked = true;
    }

    m_wasMousePressed = isPressed;
    return clicked;
}

void PlayerCardUI::render(Renderer& renderer) const {
    glm::vec4 bgColor;
    glm::vec4 accentColor;
    glm::vec4 borderColor;

    if (!m_isAffordable) {

        bgColor = glm::vec4(0.20f, 0.20f, 0.20f, 0.85f); // Dim charcoal
        accentColor = glm::vec4(0.38f, 0.38f, 0.38f, 1.0f);  // Flat gray
        borderColor = glm::vec4(0.28f, 0.28f, 0.28f, 1.0f);  // Inactive gray border
    }
    else {
        // --- COLORIZED STATE (Affordable) ---
        bgColor = glm::vec4(0.12f, 0.14f, 0.18f, 0.95f);     // Dark slate base

        if (m_unitType == CardUnitType::Melee) {
            accentColor = glm::vec4(0.90f, 0.25f, 0.25f, 1.0f); // Red
        }
        else if (m_unitType == CardUnitType::Ranged) {
            accentColor = glm::vec4(0.25f, 0.80f, 0.35f, 1.0f); // Green
        }
        else {
            accentColor = glm::vec4(0.25f, 0.55f, 0.95f, 1.0f); // Blue
        }

        if (m_isSelected) {
            borderColor = glm::vec4(1.0f, 0.84f, 0.0f, 1.0f);  // Gold ring when selected
        }
        else if (m_isHovered) {
            borderColor = glm::vec4(0.95f, 0.95f, 0.95f, 1.0f); // White ring on hover
        }
        else {
            borderColor = accentColor;
        }
    }

    // 1. Draw outer border
    renderer.DrawQuad(m_position - glm::vec2(2.0f, 2.0f), m_size + glm::vec2(4.0f, 4.0f), borderColor);

    // 2. Draw card background body
    renderer.DrawQuad(m_position, m_size, bgColor);

    // 3. Draw archetype insignia badge
    glm::vec2 iconSize = glm::vec2(m_size.x * 0.70f, m_size.y * 0.45f);
    glm::vec2 iconPos = glm::vec2(m_position.x + (m_size.x - iconSize.x) * 0.5f, m_position.y + 8.0f);
    renderer.DrawQuad(iconPos, iconSize, accentColor);

    // 4. Draw bottom cost bar
    glm::vec2 barSize = glm::vec2(m_size.x * 0.80f, 6.0f);
    glm::vec2 barPos = glm::vec2(m_position.x + (m_size.x - barSize.x) * 0.5f, m_position.y + m_size.y - 12.0f);
    glm::vec4 barColor = m_isAffordable ? glm::vec4(1.0f, 0.84f, 0.0f, 1.0f) : glm::vec4(0.35f, 0.35f, 0.35f, 1.0f);
    renderer.DrawQuad(barPos, barSize, barColor);
}