#pragma once

#include <vector>

class Environment {
private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    void generateMesh();

public:
    Environment() = default;

    void init();

    // Data getters 
    const std::vector<float>& getVertices() const { return vertices; }
    const std::vector<unsigned int>& getIndices() const { return indices; }
};