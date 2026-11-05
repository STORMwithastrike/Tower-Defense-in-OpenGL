#include "../headers/MeleePlayer.h"

MeleePlayer::MeleePlayer(int lane)
    : Player(5, lane) {
    maxHealth = 5;
    health = 5;
    advanceSpeed = 30.0f;
    canDodge = true;
}

void MeleePlayer::init(float startX, float startY) {
    position = glm::vec2(startX, startY);
    maxHealth = 5;
    health = 5;
    active = true;
    generateMesh();
}


void MeleePlayer::generateMesh() {
    vertices.clear();
    indices.clear();

    // helpeersss
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

    

    // Head 
    addQuad(10.0f, -19.0f, 16.0f, 14.0f, 0.60f, 0.65f, 0.70f);

    //  Body 
    addQuad(8.0f, -5.0f, 20.0f, 24.0f, 0.35f, 0.45f, 0.60f);

    //  Sword 
    addQuad(28.0f, 3.0f, 14.0f, 4.0f, 0.85f, 0.85f, 0.90f);
    addTriangle(42.0f, 1.0f, 42.0f, 9.0f, 48.0f, 5.0f, 0.85f, 0.85f, 0.90f);
}