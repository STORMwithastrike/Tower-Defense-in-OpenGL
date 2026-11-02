#pragma once
#include <glm/glm.hpp>

class HomeBase {
private:
    glm::vec2 m_position;
    glm::vec2 m_size;
    bool m_isBreached;

public:
    HomeBase(float startX, float startY, float width, float height);

    bool checkEnemyReached(float enemyX);

    glm::vec2 getPosition() const;
    glm::vec2 getSize() const;
    bool hasLostGame() const;
};