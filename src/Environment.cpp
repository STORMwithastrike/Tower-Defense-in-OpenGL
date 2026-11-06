#include "../headers/Environment.h"

void Environment::init() {
    generateMesh();
}

void Environment::generateMesh() {
    vertices.clear();
    indices.clear();

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

    addQuad(160.0f, 68.0f, 6.0f, 24.0f, 0.40f, 0.25f, 0.15f);
    addTriangle(157.0f, 68.0f, 169.0f, 68.0f, 163.0f, 50.0f, 0.95f, 0.50f, 0.10f);
    addTriangle(159.0f, 68.0f, 167.0f, 68.0f, 163.0f, 56.0f, 1.00f, 0.90f, 0.20f);

    addQuad(275.0f, 50.0f, 30.0f, 4.0f, 0.30f, 0.18f, 0.10f);
    addQuad(278.0f, 54.0f, 24.0f, 24.0f, 0.70f, 0.15f, 0.20f);
    addTriangle(278.0f, 78.0f, 302.0f, 78.0f, 290.0f, 92.0f, 0.70f, 0.15f, 0.20f);

    addQuad(400.0f, 66.0f, 55.0f, 28.0f, 0.45f, 0.45f, 0.48f);
    addQuad(400.0f, 54.0f, 15.0f, 12.0f, 0.50f, 0.50f, 0.53f);
    addQuad(440.0f, 54.0f, 15.0f, 12.0f, 0.50f, 0.50f, 0.53f);

    addQuad(535.0f, 50.0f, 40.0f, 5.0f, 0.35f, 0.35f, 0.38f);
    for (int i = 0; i < 3; ++i) {
        float bx = 542.0f + i * 11.0f;
        addQuad(bx, 55.0f, 4.0f, 30.0f, 0.20f, 0.20f, 0.22f);
        addTriangle(bx - 1.0f, 85.0f, bx + 5.0f, 85.0f, bx + 2.0f, 94.0f, 0.25f, 0.25f, 0.28f);
    }

    addTriangle(675.0f, 60.0f, 720.0f, 60.0f, 697.5f, 48.0f, 0.25f, 0.25f, 0.30f);
    addQuad(680.0f, 60.0f, 35.0f, 35.0f, 0.40f, 0.40f, 0.42f);
    addQuad(695.0f, 68.0f, 5.0f, 12.0f, 0.15f, 0.15f, 0.15f);
}
