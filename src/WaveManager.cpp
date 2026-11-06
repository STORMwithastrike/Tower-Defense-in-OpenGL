#include "../headers/WaveManager.h"
#include <cstdlib>

// bn-set el values w nebda2 wave 1 3ala tool
WaveManager::WaveManager()
    : currentWave(1),
    enemiesSpawned(0),
    enemiesInCurrentWave(3),
    spawnTimer(0.0f),
    spawnInterval(2.0f),
    spawningFinished(false),
    gameWon(false) {
    startWave(1);
}

// bn-reset el counters w n7aded 3adad el enemies w el delay le kol wave
void WaveManager::startWave(int waveNumber) {
    currentWave = waveNumber;
    enemiesSpawned = 0;
    spawnTimer = 0.0f;
    spawningFinished = false;
    gameWon = false;

    // kol ma el wave te3la el 3adad byzeed w el delay by2el
    if (currentWave == 1) {
        enemiesInCurrentWave = 3;
        spawnInterval = 2.0f;
    }
    else if (currentWave == 2) {
        enemiesInCurrentWave = 5;
        spawnInterval = 1.6f;
    }
    else if (currentWave == 3) {
        enemiesInCurrentWave = 7;
        spawnInterval = 1.3f;
    }
    else {
        enemiesInCurrentWave = 0;
        spawningFinished = true;
    }
}

// bnkhtar noo3 random 3ala 7asab el wave
EnemyType WaveManager::getRandomEnemyTypeForWave(int wave) {
    if (wave == 1) {
        return EnemyType::EasyUpperRanged;
    }
    else if (wave == 2) {
        int roll = std::rand() % 2;
        if (roll == 0) {
            return EnemyType::EasyUpperRanged;
        }
        else {
            return EnemyType::MediumLowerRanged;
        }
    }
    else {
        int roll = std::rand() % 3;
        if (roll == 0) {
            return EnemyType::EasyUpperRanged;
        }
        else if (roll == 1) {
            return EnemyType::MediumLowerRanged;
        }
        else {
            return EnemyType::HardMelee;
        }
    }
}

// check kol frame law ge wa2t el spawn
bool WaveManager::shouldSpawnEnemy(float dt, int numLanes, EnemyType& outType, int& outLane) {
    if (spawningFinished || gameWon || numLanes <= 0) {
        return false;
    }

    spawnTimer += dt;

    // awel ma n3addi el interval bn-spawn enemy gedid
    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.0f;

        outType = getRandomEnemyTypeForWave(currentWave);
        outLane = std::rand() % numLanes;

        enemiesSpawned++;

        // law weselna le a5er el wave nwa2af
        if (enemiesSpawned >= enemiesInCurrentWave) {
            spawningFinished = true;
        }

        return true;
    }

    return false;
}

// lma kol enemies el wave ymouto nrooh lel wave elly ba3daha
void WaveManager::onWaveEnemiesCleared() {
    if (!spawningFinished) {
        return;
    }

    if (currentWave < 3) {
        startWave(currentWave + 1);
    }
    else {
        gameWon = true;
    }
}

// getters 3ashan el UI w state el game
int WaveManager::getCurrentWave() const {
    return currentWave;
}

bool WaveManager::isSpawningFinished() const {
    return spawningFinished;
}

bool WaveManager::isGameWon() const {
    return gameWon;
}