#include "../headers/Projectile.h"

Projectile::Projectile() {
    position = glm::vec2(0.0f, 0.0f);
    speed = 0.2f;
    active = false;
}

void Projectile::init(float startX, float startY) {
    position = glm::vec2(startX, startY);
    active = true;
    generateMesh();
}

void Projectile::generateMesh() {
    vertices.clear();
    indices.clear();

    // Colored rectangle helperr
    auto addQuad = [this](float x, float y, float w, float h, float r, float g, float b) {
        unsigned int base = static_cast<unsigned int>(vertices.size() / 6);
        float qVerts[] = {
            x,     y,     0.0f, r, g, b,
            x + w, y,     0.0f, r, g, b,
            x,     y + h, 0.0f, r, g, b,
            x + w, y + h, 0.0f, r, g, b
        };
        vertices.insert(vertices.end(), std::begin(qVerts), std::end(qVerts));

        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        indices.push_back(base + 1);
        indices.push_back(base + 3);
        indices.push_back(base + 2);
        };

    // Colored triangle helperr
    auto addTriangle = [this](float x1, float y1, float x2, float y2, float x3, float y3, float r, float g, float b) {
        unsigned int base = static_cast<unsigned int>(vertices.size() / 6);
        float tVerts[] = {
            x1, y1, 0.0f, r, g, b,
            x2, y2, 0.0f, r, g, b,
            x3, y3, 0.0f, r, g, b
        };
        vertices.insert(vertices.end(), std::begin(tVerts), std::end(tVerts));

        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        };

    // Shaft (brown rectangle)
    addQuad(0.0f, 3.0f, 26.0f, 6.0f, 0.55f, 0.35f, 0.20f);

    // Arrowhead (gray tip )
    addTriangle(26.0f, 0.0f, 26.0f, 12.0f, 38.0f, 6.0f, 0.20f, 0.20f, 0.22f);
}

void Projectile::update() {
    position.x += speed;
    if (position.x > 800.0f) {
        active = false;
    }
}