#include "../headers/WaveManager.h"
#include <cstdlib>

WaveManager::WaveManager()
    : m_currentWave(1),
    m_enemiesSpawned(0),
    m_enemiesInCurrentWave(3),
    m_spawnTimer(0.0f),
    m_spawnInterval(2.0f),
    m_isSpawningFinished(false),
    m_isGameWon(false) {
    startWave(1);
}

void WaveManager::startWave(int waveNumber) {
    m_currentWave = waveNumber;
    m_enemiesSpawned = 0;
    m_spawnTimer = 0.0f;
    m_isSpawningFinished = false;

    if (m_currentWave == 1) {
        m_enemiesInCurrentWave = 3;
        m_spawnInterval = 2.0f; // Fast opening wave
    }
    else if (m_currentWave == 2) { // Fixed == comparison
        m_enemiesInCurrentWave = 5;
        m_spawnInterval = 1.6f; // Snappier spawns so wave 2 doesn't drag
    }
    else if (m_currentWave == 3) { // Fixed == comparison
        m_enemiesInCurrentWave = 7; // Tuned down from 10 to keep the final push intense but brisk
        m_spawnInterval = 1.3f;
    }
    else {
        m_enemiesInCurrentWave = 0;
        m_isSpawningFinished = true;
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
        // Wave 3 mixes all three enemy types
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
    if (m_isSpawningFinished || m_isGameWon || numLanes <= 0) {
        return false;
    }

    m_spawnTimer += dt;

    if (m_spawnTimer >= m_spawnInterval) {
        m_spawnTimer = 0.0f;

        outType = getRandomEnemyTypeForWave(m_currentWave);
        outLane = std::rand() % numLanes;

        m_enemiesSpawned++;

        if (m_enemiesSpawned >= m_enemiesInCurrentWave) {
            m_isSpawningFinished = true;
        }

        return true;
    }

    return false;
}

void WaveManager::onWaveEnemiesCleared() {
    if (!m_isSpawningFinished) {
        return;
    }

    if (m_currentWave < 3) {
        startWave(m_currentWave + 1);
    }
    else {
        m_isGameWon = true;
    }
}

int WaveManager::getCurrentWave() const {
    return m_currentWave;
}

bool WaveManager::isSpawningFinished() const {
    return m_isSpawningFinished;
}

bool WaveManager::isGameWon() const {
    return m_isGameWon;
}