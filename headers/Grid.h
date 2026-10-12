#pragma once

#include <vector>

struct Lane {
    int id;
    float minY;
    float maxY;
    float centerY;
    float minX;
    float maxX;
};

class Grid {
private:
    std::vector<Lane> lanes;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    void generateMesh();

public:
    Grid();

    void init();

    //getters hena
    const std::vector<float>& getVertices() const { return vertices; }
    const std::vector<unsigned int>& getIndices() const { return indices; }

    //helpers lel other members
    const std::vector<Lane>& getLanes() const { return lanes; }
    int getLaneCount() const { return static_cast<int>(lanes.size()); }
    float getLaneCenterY(int laneIndex) const;
};