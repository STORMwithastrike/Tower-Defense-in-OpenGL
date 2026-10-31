#include "../headers/DefensePlayer.h"

DefensePlayer::DefensePlayer(int lane)
    : Player(7, lane) { // 7 hits
    advanceSpeed = 0.0f; // dosnt move
    canDodge = false; // or jump
}

void DefensePlayer::init(float startX, float startY) {
    position = glm::vec2(startX, startY);
    generateMesh();
}

void DefensePlayer::generateMesh() {
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

    //Head 
    addQuad(8.0f, 6.0f, 16.0f, 12.0f, 0.40f, 0.42f, 0.48f);

    //Body 
    addQuad(6.0f, 18.0f, 18.0f, 24.0f, 0.30f, 0.32f, 0.38f);

    //Shield 
    addQuad(24.0f, 8.0f, 12.0f, 32.0f, 0.50f, 0.52f, 0.58f);
    addTriangle(24.0f, 40.0f, 36.0f, 40.0f, 30.0f, 48.0f, 0.50f, 0.52f, 0.58f);
}