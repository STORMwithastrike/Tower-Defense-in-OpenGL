#include "../headers/MeleePlayer.h"

MeleePlayer::MeleePlayer(int lane)
    : Player(5, lane) { // 5 lives
    advanceSpeed = 30.0f;
    canDodge = true;
}

void MeleePlayer::init(float startX, float startY) {
    position = glm::vec2(startX, startY);
    generateMesh();
}

void MeleePlayer::generateMesh() {
    vertices.clear();
    indices.clear();

    // Quad helper (x, y, width, height, r, g, b)
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

    // Triangle helper (x1, y1, x2, y2, x3, y3, r, g, b)
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

    // 1. Head (Gray Helmet)
    addQuad(10.0f, 4.0f, 16.0f, 14.0f, 0.60f, 0.65f, 0.70f);

    // 2. Body (Blueish Armor)
    addQuad(8.0f, 18.0f, 20.0f, 24.0f, 0.35f, 0.45f, 0.60f);

    // 3. Sword (Blade + Tip)
    addQuad(28.0f, 26.0f, 14.0f, 4.0f, 0.85f, 0.85f, 0.90f);
    addTriangle(42.0f, 24.0f, 42.0f, 32.0f, 48.0f, 28.0f, 0.85f, 0.85f, 0.90f);
}