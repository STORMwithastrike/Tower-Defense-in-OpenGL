#include "../headers/PlayerCardUI.h"

PlayerCardUI::PlayerCardUI(CardUnitType unitType, int cost, const glm::vec2& position, const glm::vec2& size)
    : unitType(unitType),
    cost(cost),
    position(position),
    size(size),
    affordable(false),
    hovered(false),
    selected(false),
    wasMousePressed(false) {
}

bool PlayerCardUI::contains(double x, double y) const {
    return (x >= position.x && x <= position.x + size.x &&
        y >= position.y && y <= position.y + size.y);
}

void PlayerCardUI::update(const Economy& economy, double mouseX, double mouseY) {
    affordable = economy.canAfford(cost);
    hovered = contains(mouseX, mouseY);
}

bool PlayerCardUI::handleMouseClick(double mouseX, double mouseY, bool isPressed) {
    bool clicked = false;

    if (isPressed && !wasMousePressed && hovered && affordable) {
        selected = !selected;
        clicked = true;
    }

    wasMousePressed = isPressed;
    return clicked;
}

void PlayerCardUI::render(Renderer& renderer) const {
    glm::vec4 bgColor;
    glm::vec4 accentColor;
    glm::vec4 borderColor;

    if (!affordable) {
        bgColor = glm::vec4(0.20f, 0.20f, 0.20f, 0.85f);
        accentColor = glm::vec4(0.38f, 0.38f, 0.38f, 1.0f);
        borderColor = glm::vec4(0.28f, 0.28f, 0.28f, 1.0f);
    }
    else {
        bgColor = glm::vec4(0.12f, 0.14f, 0.18f, 0.95f);

        if (unitType == CardUnitType::Melee) {
            accentColor = glm::vec4(0.90f, 0.25f, 0.25f, 1.0f);
        }
        else if (unitType == CardUnitType::Ranged) {
            accentColor = glm::vec4(0.25f, 0.80f, 0.35f, 1.0f);
        }
        else {
            accentColor = glm::vec4(0.25f, 0.55f, 0.95f, 1.0f);
        }

        if (selected) {
            borderColor = glm::vec4(1.0f, 0.84f, 0.0f, 1.0f);
        }
        else if (hovered) {
            borderColor = glm::vec4(0.95f, 0.95f, 0.95f, 1.0f);
        }
        else {
            borderColor = accentColor;
        }
    }

    renderer.DrawQuad(position - glm::vec2(2.0f, 2.0f), size + glm::vec2(4.0f, 4.0f), borderColor);
    renderer.DrawQuad(position, size, bgColor);

    renderMiniModel(renderer);

    glm::vec2 barSize = glm::vec2(size.x * 0.80f, 6.0f);
    glm::vec2 barPos = glm::vec2(position.x + (size.x - barSize.x) * 0.5f, position.y + size.y - 12.0f);
    glm::vec4 barColor = affordable ? glm::vec4(1.0f, 0.84f, 0.0f, 1.0f) : glm::vec4(0.35f, 0.35f, 0.35f, 1.0f);
    renderer.DrawQuad(barPos, barSize, barColor);
}

void PlayerCardUI::renderMiniModel(Renderer& renderer) const {
    const float scale = 0.5f;
    const float centerX = position.x + size.x * 0.5f;
    const float centerY = position.y + (size.y - 12.0f) * 0.5f;

    auto makeColor = [this](float r, float g, float b) -> glm::vec4 {
        if (!affordable) {
            float gray = 0.299f * r + 0.587f * g + 0.114f * b;
            return glm::vec4(gray, gray, gray, 1.0f);
        }
        return glm::vec4(r, g, b, 1.0f);
        };

    auto drawMiniQuad = [&](float origX, float origY, float origW, float origH, float modelCenterX, float r, float g, float b) {
        glm::vec2 pos(centerX + (origX - modelCenterX) * scale, centerY + origY * scale);
        glm::vec2 sz(origW * scale, origH * scale);
        renderer.DrawQuad(pos, sz, makeColor(r, g, b));
        };

    auto drawMiniTriangle = [&](float x1, float y1, float x2, float y2, float x3, float y3, float modelCenterX, float r, float g, float b) {
        glm::vec2 p1(centerX + (x1 - modelCenterX) * scale, centerY + y1 * scale);
        glm::vec2 p2(centerX + (x2 - modelCenterX) * scale, centerY + y2 * scale);
        glm::vec2 p3(centerX + (x3 - modelCenterX) * scale, centerY + y3 * scale);
        renderer.DrawTriangle(p1, p2, p3, makeColor(r, g, b));
        };

    if (unitType == CardUnitType::Melee) {
        const float modelCenterX = 28.0f;
        drawMiniQuad(10.0f, -19.0f, 16.0f, 14.0f, modelCenterX, 0.60f, 0.65f, 0.70f);
        drawMiniQuad(8.0f, -5.0f, 20.0f, 24.0f, modelCenterX, 0.35f, 0.45f, 0.60f);
        drawMiniQuad(28.0f, 3.0f, 14.0f, 4.0f, modelCenterX, 0.85f, 0.85f, 0.90f);
        drawMiniTriangle(42.0f, 1.0f, 42.0f, 9.0f, 48.0f, 5.0f, modelCenterX, 0.85f, 0.85f, 0.90f);
    }
    else if (unitType == CardUnitType::Ranged) {
        const float modelCenterX = 26.0f;
        drawMiniQuad(9.0f, -19.0f, 16.0f, 14.0f, modelCenterX, 0.30f, 0.30f, 0.35f);
        drawMiniQuad(8.0f, -5.0f, 18.0f, 24.0f, modelCenterX, 0.52f, 0.35f, 0.20f);
        drawMiniQuad(26.0f, 3.0f, 18.0f, 4.0f, modelCenterX, 0.24f, 0.14f, 0.08f);
        drawMiniQuad(38.0f, -5.0f, 4.0f, 20.0f, modelCenterX, 0.30f, 0.30f, 0.35f);
    }
    else if (unitType == CardUnitType::Defense) {
        const float modelCenterX = 21.0f;
        drawMiniQuad(8.0f, -21.0f, 16.0f, 12.0f, modelCenterX, 0.40f, 0.42f, 0.48f);
        drawMiniQuad(6.0f, -9.0f, 18.0f, 24.0f, modelCenterX, 0.30f, 0.32f, 0.38f);
        drawMiniQuad(24.0f, -19.0f, 12.0f, 32.0f, modelCenterX, 0.50f, 0.52f, 0.58f);
        drawMiniTriangle(24.0f, 13.0f, 36.0f, 13.0f, 30.0f, 21.0f, modelCenterX, 0.50f, 0.52f, 0.58f);
    }
}