#pragma once

#include <glm/glm.hpp>

// el base / el qal3a elly bn7meeha mn el enemies
class HomeBase {
private:
    glm::vec2 position;   // makan el base
    glm::vec2 size;       // ab3ad / 7agm el base
    bool isBreached;      // true lw enemy wesel w 3adda
    int currentHealth;    // dam el base el 7aly
    int maxHealth;        // max hp lel base

public:
    HomeBase(float startX, float startY, float width, float height, int maxHp = 10);

    // check lw el enemy wesel le 7dood el base
    bool checkEnemyReached(float enemyX);

    // bten2as hp lma takhod hit
    void takeDamage(int amount);

    // getters 3ashan el render wel bounds
    glm::vec2 getPosition() const;
    glm::vec2 getSize() const;

    // check lw dam el base 5eles w khesrna
    bool hasLostGame() const;

    int getHealth() const { return currentHealth; }
    int getMaxHealth() const { return maxHealth; }

    // reset lel hp w el state lma el game yebda2 mn gedeed
    void reset();
};