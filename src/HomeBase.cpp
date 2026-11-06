#include "../headers/HomeBase.h"

// bn-set el makan wel 7agm wel max hp fl bedaya
HomeBase::HomeBase(float startX, float startY, float width, float height, int maxHp)
    : position(startX, startY),
    size(width, height),
    isBreached(false),
    currentHealth(maxHp),
    maxHealth(maxHp) {
}

// lw el enemy wesel le 7dood el base btekhod 3 damage w traga3 true
bool HomeBase::checkEnemyReached(float enemyX) {
    if (enemyX <= (position.x + size.x)) {
        takeDamage(3);
        return true;
    }
    return false;
}

// bten2as hp w lw wesel zero yeb2a khalas etkasaret
void HomeBase::takeDamage(int amount) {
    currentHealth -= amount;
    if (currentHealth <= 0) {
        currentHealth = 0;
        isBreached = true;
    }
}

// reset lel hp lma el game y-restart
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

// check lw kheserna (dam el base 5eles aw etkasaret)
bool HomeBase::hasLostGame() const {
    return isBreached || currentHealth <= 0;
}