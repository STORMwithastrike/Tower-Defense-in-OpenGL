#pragma once

#include <glm/glm.hpp>

class HomeBase {
private:
    glm::vec2 position;
    glm::vec2 size;
    bool isBreached;
    int currentHealth;
    int maxHealth;

public:
    HomeBase(float startX, float startY, float width, float height, int maxHp = 10);

    bool checkEnemyReached(float enemyX);
    void takeDamage(int amount);

    glm::vec2 getPosition() const;
    glm::vec2 getSize() const;
    bool hasLostGame() const;

    int getHealth() const { return currentHealth; }
    int getMaxHealth() const { return maxHealth; }
    void reset();
};