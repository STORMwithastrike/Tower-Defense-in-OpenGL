#include "../headers/Grid.h"

Grid::Grid() {
}

void Grid::init() {
    generateMesh();
}

float Grid::getLaneCenterY(int laneIndex) const {
    return lanes[laneIndex].centerY;
}

void Grid::generateMesh() {
    vertices.clear();
    indices.clear();
    lanes.clear();

    const int numLanes = 4;
    const float minX = 100.0f;
    const float maxX = 800.0f;
    const float startY = 100.0f;
    const float laneHeight = 110.0f;
    const float dividerThickness = 2.0f;

    // Helper 3shan a push 1 rectangle (4 vertices, 6 indices)
    auto addQuad = [this](float x0, float x1, float y0, float y1, float r, float g, float b) {
        unsigned int baseIndex = static_cast<unsigned int>(vertices.size() / 6);

        // 4 Vertices (x, y, z, r, g, b)
        float quadVertices[] = {
            x0, y0, 0.0f,  r, g, b, // 0: Top-Left
            x1, y0, 0.0f,  r, g, b, // 1: Top-Right
            x0, y1, 0.0f,  r, g, b, // 2: Bottom-Left
            x1, y1, 0.0f,  r, g, b  // 3: Bottom-Right
        };
        vertices.insert(vertices.end(), std::begin(quadVertices), std::end(quadVertices));

        // 6 Indices lel 2 Triangles el by3mel el rectangle
        indices.push_back(baseIndex + 0);
        indices.push_back(baseIndex + 1);
        indices.push_back(baseIndex + 2);

        indices.push_back(baseIndex + 1);
        indices.push_back(baseIndex + 3);
        indices.push_back(baseIndex + 2);
        };

    for (int i = 0; i < numLanes; ++i) {
        float y0 = startY + i * laneHeight;
        float y1 = y0 + laneHeight;

        // Store lane metrics lel teammates
        Lane lane;
        lane.id = i;
        lane.minX = minX;
        lane.maxX = maxX;
        lane.minY = y0;
        lane.maxY = y1;
        lane.centerY = y0 + (laneHeight / 2.0f);
        lanes.push_back(lane);

        // Alternating colors 
        float r = (i % 2 == 0) ? 0.35f : 0.45f;
        float g = (i % 2 == 0) ? 0.35f : 0.45f;
        float b = (i % 2 == 0) ? 0.38f : 0.48f;

        // Add the main lane rectangle
        addQuad(minX, maxX, y0, y1, r, g, b);

        //  line divider el t7t el lane
        addQuad(minX, maxX, y1 - dividerThickness, y1, 0.20f, 0.20f, 0.22f);
    }
}