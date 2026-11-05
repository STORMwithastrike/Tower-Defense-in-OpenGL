#include "../headers/Economy.h"

Economy::Economy(int startingCoins)
    : coins(startingCoins),
    startingCoins(startingCoins) {
}

void Economy::addCoins(int amount) {
    if (amount > 0) {
        coins += amount;
    }
}

bool Economy::spendCoins(int cost) {
    if (cost < 0) return false;

    if (canAfford(cost)) {
        coins -= cost;
        return true;
    }
    return false;
}

void Economy::setCoins(int amount) {
    coins = (amount >= 0) ? amount : 0;
}

void Economy::reset() {
    coins = startingCoins;
}