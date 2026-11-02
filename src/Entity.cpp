#include "../headers/Entity.h"
#include <algorithm>

Entity::Entity(int maxHp, int lane)
    : position(0.0f, 0.0f), laneId(lane), currentHealth(maxHp), maxHealth(maxHp), active(true) {
}

void Entity::takeDamage(int amount) {
    currentHealth -= amount;
    if (currentHealth <= 0) {
        currentHealth = 0;
        active = false;
    }
}

bool Entity::isAlive() const {
    return currentHealth > 0 && active;
}

float Entity::getHealthRatio() const {
    if (maxHealth <= 0) return 0.0f;
    return std::clamp(static_cast<float>(currentHealth) / static_cast<float>(maxHealth), 0.0f, 1.0f);
}