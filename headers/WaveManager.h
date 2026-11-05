#pragma once

enum class EnemyType {
    EasyUpperRanged,
    MediumLowerRanged,
    HardMelee
};

class WaveManager {
private:
    int currentWave;
    int enemiesSpawned;
    int enemiesInCurrentWave;
    float spawnTimer;
    float spawnInterval;
    bool spawningFinished;
    bool gameWon;

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