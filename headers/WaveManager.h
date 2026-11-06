#pragma once

// anwa3 el enemies elly 3andena
enum class EnemyType {
    EasyUpperRanged,
    MediumLowerRanged,
    HardMelee
};

class WaveManager {
private:
    int currentWave;           // el wave el 7aleya
    int enemiesSpawned;        // kam enemy nzel l7ad delwa2ty
    int enemiesInCurrentWave;  // el total bta3 el wave de
    float spawnTimer;          // timer ben el spawns
    float spawnInterval;       // el wa2t elly bnstnah ben kol wa7ed
    bool spawningFinished;     // khalas el wave nazlet kolaha
    bool gameWon;              // kesebna kol el waves

    // bykhtar noo3 random 3ala 7asab el wave
    EnemyType getRandomEnemyTypeForWave(int wave);

public:
    WaveManager();

    // btbda2 wave gdeeda
    void startWave(int waveNumber);

    // bte7seb el wa2t w teshof law 7ad hayenzel
    bool shouldSpawnEnemy(float dt, int numLanes, EnemyType& outType, int& outLane);

    // btetnadah lama kol enemies el wave ymouto
    void onWaveEnemiesCleared();

    // getters 3ashan el UI w state el game
    int getCurrentWave() const;
    bool isSpawningFinished() const;
    bool isGameWon() const;
};