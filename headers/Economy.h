#pragma once

class Economy {
public:
    explicit Economy(int startingCoins = 60);

    // Coin transactions
    void addCoins(int amount);
    void AddCoins(int amount) { addCoins(amount); }

    // Defined inline in the header to guarantee the linker finds it
    bool canAfford(int cost) const { return m_coins >= cost; }
    bool CanAfford(int cost) const { return canAfford(cost); }

    bool spendCoins(int cost);
    bool SpendCoins(int cost) { return spendCoins(cost); }

    // State inspection
    int getCoins() const { return m_coins; }
    int GetCoins() const { return m_coins; }
    void setCoins(int amount);
    void reset();

private:
    int m_coins;
    int m_startingCoins;
};