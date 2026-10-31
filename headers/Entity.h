#pragma once

#include <glm/glm.hpp>
#include <vector>

class Entity {
protected:
    glm::vec2 position;
    int laneId;
    int currentHealth;
    int maxHealth;
    bool active;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

public:
    Entity(int maxHp = 5, int lane = 0);

    void takeDamage(int amount);
    bool isAlive() const;
    float getHealthRatio() const;

    glm::vec2 getPosition() const { return position; }
    void setPosition(const glm::vec2& pos) { position = pos; }
    int getLaneId() const { return laneId; }
    void setLaneId(int lane) { laneId = lane; }

    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }

    const std::vector<float>& getVertices() const { return vertices; }
    const std::vector<unsigned int>& getIndices() const { return indices; }
};