#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "WaveManager.h"

class Enemy {
private:
    EnemyType m_type;
    int m_laneId;
    glm::vec2 m_position;
    glm::vec2 m_size;
    float m_speed;
    int m_lives;
    int m_maxLives;
    bool m_isAlive;

    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;

    void addBox(float x, float y, float w, float h, float r, float g, float b);
    void buildMesh();

public:
    Enemy(EnemyType type, int laneId, float startX, float startY);

    void update(float dt);
    void takeDamage(int amount);

    glm::vec2 getPosition() const;
    glm::vec2 getSize() const;
    int getLaneId() const;
    int getLives() const;
    int getMaxLives() const;
    bool isAlive() const;
    EnemyType getType() const;

    const std::vector<float>& getVertices() const;
    const std::vector<unsigned int>& getIndices() const;
};