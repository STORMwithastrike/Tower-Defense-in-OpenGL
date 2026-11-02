#pragma once

#include <vector>
#include <glm/glm.hpp>

class Projectile {
private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    glm::vec2 position;
    float speed;
    int damage;
    bool active;

    void generateMesh();

public:
    Projectile();

    void init(float startX, float startY);
    void update();

    const std::vector<float>& getVertices() const { return vertices; }
    const std::vector<unsigned int>& getIndices() const { return indices; }

    glm::vec2 getPosition() const { return position; }
    void setPosition(glm::vec2 pos) { position = pos; }

    int getDamage() const { return damage; }
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }
};