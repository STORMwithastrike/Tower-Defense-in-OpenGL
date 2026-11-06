#pragma once

// economy system mas2ool 3an flous el player
class Economy {
private:
    int coins;          // el gold el 7aly
    int startingCoins;  // flous el bedaya (default 60)

public:
    explicit Economy(int startingCoins = 60);

    // bnzawed coins lama nlem drop aw neksab wave
    void addCoins(int amount);
    void AddCoins(int amount) { addCoins(amount); }

    // check lw ma3ana flous tekfy el unit
    bool canAfford(int cost) const { return coins >= cost; }
    bool CanAfford(int cost) const { return canAfford(cost); }

    // bnkhesm el se3r lw ma3ana kfaya
    bool spendCoins(int cost);
    bool SpendCoins(int cost) { return spendCoins(cost); }

    // getters lel HUD w cards el UI
    int getCoins() const { return coins; }
    int GetCoins() const { return coins; }

    // setter w reset lel flous lama el game y-restart
    void setCoins(int amount);
    void reset();
};