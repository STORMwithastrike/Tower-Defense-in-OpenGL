#include "../headers/WaveManager.h"
#include <cstdlib>

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

void WaveManager::startWave(int waveNumber) {
    currentWave = waveNumber;
    enemiesSpawned = 0;
    spawnTimer = 0.0f;
    spawningFinished = false;
    gameWon = false;

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

bool WaveManager::shouldSpawnEnemy(float dt, int numLanes, EnemyType& outType, int& outLane) {
    if (spawningFinished || gameWon || numLanes <= 0) {
        return false;
    }

    spawnTimer += dt;

    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.0f;

        outType = getRandomEnemyTypeForWave(currentWave);
        outLane = std::rand() % numLanes;

        enemiesSpawned++;

        if (enemiesSpawned >= enemiesInCurrentWave) {
            spawningFinished = true;
        }

        return true;
    }

    return false;
}

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

int WaveManager::getCurrentWave() const {
    return currentWave;
}

bool WaveManager::isSpawningFinished() const {
    return spawningFinished;
}

bool WaveManager::isGameWon() const {
    return gameWon;
}