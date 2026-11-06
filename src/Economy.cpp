#include "../headers/Economy.h"

// bn-set el coins wel starting coins bel value elly bnbda2 beeha
Economy::Economy(int startingCoins)
    : coins(startingCoins),
    startingCoins(startingCoins) {
}

// bnzawed gold lama nlem coin w bn-check en el amount positive
void Economy::addCoins(int amount) {
    if (amount > 0) {
        coins += amount;
    }
}

// bnkhesm se3r el hero lw ma3ana kfaya w nragga3 true
bool Economy::spendCoins(int cost) {
    if (cost < 0) return false;

    if (canAfford(cost)) {
        coins -= cost;
        return true;
    }
    return false;
}

// direct setter lel coins w my2elsh 3an zero
void Economy::setCoins(int amount) {
    coins = (amount >= 0) ? amount : 0;
}

// reset lel flous bta3et el bedaya lma el game y-restart
void Economy::reset() {
    coins = startingCoins;
}