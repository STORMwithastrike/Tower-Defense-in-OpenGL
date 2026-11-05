#pragma once

class Economy {
private:
    int coins;
    int startingCoins;

public:
    explicit Economy(int startingCoins = 60);

    void addCoins(int amount);
    void AddCoins(int amount) { addCoins(amount); }

    bool canAfford(int cost) const { return coins >= cost; }
    bool CanAfford(int cost) const { return canAfford(cost); }

    bool spendCoins(int cost);
    bool SpendCoins(int cost) { return spendCoins(cost); }

    int getCoins() const { return coins; }
    int GetCoins() const { return coins; }
    void setCoins(int amount);
    void reset();
};