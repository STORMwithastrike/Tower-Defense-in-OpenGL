#include "../headers/HomeBase.h"

HomeBase::HomeBase(float startX, float startY, float width, float height, int maxHp)
    : position(startX, startY),
    size(width, height),
    isBreached(false),
    currentHealth(maxHp),
    maxHealth(maxHp) {
}

bool HomeBase::checkEnemyReached(float enemyX) {
    if (enemyX <= (position.x + size.x)) {
        takeDamage(3);
        return true;
    }
    return false;
}

void HomeBase::takeDamage(int amount) {
    currentHealth -= amount;
    if (currentHealth <= 0) {
        currentHealth = 0;
        isBreached = true;
    }
}

void HomeBase::reset() {
    currentHealth = maxHealth;
    isBreached = false;
}

glm::vec2 HomeBase::getPosition() const {
    return position;
}

glm::vec2 HomeBase::getSize() const {
    return size;
}

bool HomeBase::hasLostGame() const {
    return isBreached || currentHealth <= 0;
}