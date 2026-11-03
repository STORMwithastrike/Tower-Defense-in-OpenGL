#pragma once
#include <glm/glm.hpp>

class HomeBase {
private:
    glm::vec2 m_position;
    glm::vec2 m_size;
    bool m_isBreached;
    int m_currentHealth;
    int m_maxHealth;

public:
    HomeBase(float startX, float startY, float width, float height, int maxHp = 10);

    bool checkEnemyReached(float enemyX);
    void takeDamage(int amount);

    glm::vec2 getPosition() const;
    glm::vec2 getSize() const;
    bool hasLostGame() const;

    int getHealth() const { return m_currentHealth; }
    int getMaxHealth() const { return m_maxHealth; }
    void reset();
};
