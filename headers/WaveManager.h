#pragma once

enum class EnemyType {
    EasyUpperRanged,
    MediumLowerRanged,
    HardMelee
};

class WaveManager {
private:
    int m_currentWave;
    int m_enemiesSpawned;
    int m_enemiesInCurrentWave;
    float m_spawnTimer;
    float m_spawnInterval;
    bool m_isSpawningFinished;
    bool m_isGameWon;

    EnemyType getRandomEnemyTypeForWave(int wave);

public:
    WaveManager();

    void startWave(int waveNumber);
    bool shouldSpawnEnemy(float dt, int numLanes, EnemyType& outType, int& outLane);
    void onWaveEnemiesCleared();

    int getCurrentWave() const;
    bool isSpawningFinished() const;
    bool isGameWon() const;
};