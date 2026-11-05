#include "../headers/Environment.h"

void Environment::init() {
    generateMesh();
}

void Environment::generateMesh() {
    vertices.clear();
    indices.clear();

    // el helpers
    auto addQuad = [this](float x, float y, float w, float h, float r, float g, float b) {
        unsigned int base = static_cast<unsigned int>(vertices.size() / 6);
        float qVerts[] = {
            x,     y,     0.0f, r, g, b, // Top-Left
            x + w, y,     0.0f, r, g, b, // Top_right
            x,     y + h, 0.0f, r, g, b, // Bottom-left
            x + w, y + h, 0.0f, r, g, b  // Bottom_Right
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

    // Torch 
    addQuad(160.0f, 565.0f, 6.0f, 20.0f, 0.40f, 0.25f, 0.15f); // Wood
    addTriangle(157.0f, 565.0f, 169.0f, 565.0f, 163.0f, 548.0f, 0.95f, 0.50f, 0.10f); //  firee
    addTriangle(159.0f, 565.0f, 167.0f, 565.0f, 163.0f, 553.0f, 1.00f, 0.90f, 0.20f); // core

    // Banner 
    addQuad(275.0f, 548.0f, 30.0f, 4.0f, 0.30f, 0.18f, 0.10f);  // Wood bar 
    addQuad(278.0f, 552.0f, 24.0f, 25.0f, 0.70f, 0.15f, 0.20f); // bannerr
    addTriangle(278.0f, 577.0f, 302.0f, 577.0f, 290.0f, 590.0f, 0.70f, 0.15f, 0.20f); //  bottom

    // Defensive Wall 
    addQuad(400.0f, 568.0f, 55.0f, 25.0f, 0.45f, 0.45f, 0.48f); //  Main 
    addQuad(400.0f, 558.0f, 15.0f, 10.0f, 0.50f, 0.50f, 0.53f); // Left 
    addQuad(440.0f, 558.0f, 15.0f, 10.0f, 0.50f, 0.50f, 0.53f); // Right 

    // Iron Gate 
    addQuad(535.0f, 550.0f, 40.0f, 5.0f, 0.35f, 0.35f, 0.38f); // Stone 
    for (int i = 0; i < 3; ++i) {
        float bx = 542.0f + i * 11.0f;
        addQuad(bx, 555.0f, 4.0f, 25.0f, 0.20f, 0.20f, 0.22f); // Iron  bar
        addTriangle(bx - 1.0f, 580.0f, bx + 5.0f, 580.0f, bx + 2.0f, 587.0f, 0.25f, 0.25f, 0.28f); // Spike
    }

    // Watchtower 
    addQuad(680.0f, 555.0f, 35.0f, 38.0f, 0.40f, 0.40f, 0.42f); //  base
    addTriangle(675.0f, 555.0f, 720.0f, 555.0f, 697.5f, 542.0f, 0.25f, 0.25f, 0.30f); //  roof
    addQuad(695.0f, 565.0f, 5.0f, 12.0f, 0.15f, 0.15f, 0.15f);  // Arrow 
}