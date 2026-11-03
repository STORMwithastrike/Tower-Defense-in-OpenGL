#include "../headers/Economy.h"

Economy::Economy(int startingCoins)
    : m_coins(startingCoins),
    m_startingCoins(startingCoins) {
}

void Economy::addCoins(int amount) {
    if (amount > 0) {
        m_coins += amount;
    }
}

bool Economy::spendCoins(int cost) {
    if (cost < 0) return false;

    if (canAfford(cost)) {
        m_coins -= cost;
        return true;
    }
    return false;
}

void Economy::setCoins(int amount) {
    m_coins = (amount >= 0) ? amount : 0;
}

void Economy::reset() {
    m_coins = m_startingCoins;
}