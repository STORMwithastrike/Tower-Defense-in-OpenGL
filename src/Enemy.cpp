#include "../headers/Enemy.h"

Enemy::Enemy(EnemyType type, int laneId, float startX, float startY)
    : m_type(type),
    m_laneId(laneId),
    m_position(startX, startY),
    m_size(45.0f, 50.0f),
    m_isAlive(true) {

    if (m_type == EnemyType::EasyUpperRanged) {
        m_speed = 70.0f;
        m_lives = 5;
        m_maxLives = 5;
    }
    else if (m_type == EnemyType::MediumLowerRanged) {
        m_speed = 70.0f;
        m_lives = 5;
        m_maxLives = 5;
    }
    else { // HardMelee
        m_speed = 120.0f;
        m_lives = 7;
        m_maxLives = 7;
    }

    buildMesh();
}

void Enemy::addBox(float x, float y, float w, float h, float r, float g, float b) {
    unsigned int baseIndex = static_cast<unsigned int>(m_vertices.size() / 6);

    // Vertex 0: Top-Left
    m_vertices.push_back(x);     m_vertices.push_back(y);     m_vertices.push_back(0.0f);
    m_vertices.push_back(r);     m_vertices.push_back(g);     m_vertices.push_back(b);

    // Vertex 1: Top-Right
    m_vertices.push_back(x + w); m_vertices.push_back(y);     m_vertices.push_back(0.0f);
    m_vertices.push_back(r);     m_vertices.push_back(g);     m_vertices.push_back(b);

    // Vertex 2: Bottom-Right
    m_vertices.push_back(x + w); m_vertices.push_back(y + h); m_vertices.push_back(0.0f);
    m_vertices.push_back(r);     m_vertices.push_back(g);     m_vertices.push_back(b);

    // Vertex 3: Bottom-Left
    m_vertices.push_back(x);     m_vertices.push_back(y + h); m_vertices.push_back(0.0f);
    m_vertices.push_back(r);     m_vertices.push_back(g);     m_vertices.push_back(b);

    // Triangles
    m_indices.push_back(baseIndex + 0);
    m_indices.push_back(baseIndex + 1);
    m_indices.push_back(baseIndex + 2);

    m_indices.push_back(baseIndex + 2);
    m_indices.push_back(baseIndex + 3);
    m_indices.push_back(baseIndex + 0);
}

void Enemy::buildMesh() {
    m_vertices.clear();
    m_indices.clear();

    if (m_type == EnemyType::EasyUpperRanged) {
        // Torso
        addBox(12.0f, 15.0f, 24.0f, 32.0f, 0.70f, 0.18f, 0.18f);
        // Head / Helmet
        addBox(15.0f, 0.0f, 18.0f, 15.0f, 0.48f, 0.12f, 0.12f);
        // High weapon pointing left
        addBox(-6.0f, 2.0f, 18.0f, 6.0f, 0.85f, 0.85f, 0.90f);
    }
    else if (m_type == EnemyType::MediumLowerRanged) {
        // Torso
        addBox(12.0f, 15.0f, 24.0f, 32.0f, 0.65f, 0.35f, 0.12f);
        // Head / Helmet
        addBox(15.0f, 0.0f, 18.0f, 15.0f, 0.42f, 0.22f, 0.08f);
        // Low weapon pointing left
        addBox(-6.0f, 36.0f, 18.0f, 6.0f, 0.85f, 0.85f, 0.90f);
    }
    else { // HardMelee
        // Torso
        addBox(8.0f, 12.0f, 32.0f, 36.0f, 0.42f, 0.12f, 0.52f);
        // Helm
        addBox(14.0f, 0.0f, 20.0f, 14.0f, 0.25f, 0.06f, 0.32f);
        // Horns
        addBox(11.0f, -6.0f, 5.0f, 7.0f, 0.85f, 0.75f, 0.25f);
        addBox(32.0f, -6.0f, 5.0f, 7.0f, 0.85f, 0.75f, 0.25f);
        // Cleaver
        addBox(-8.0f, 16.0f, 16.0f, 18.0f, 0.80f, 0.80f, 0.85f);
    }
}

void Enemy::update(float dt) {
    if (!m_isAlive) {
        return;
    }

    m_position.x -= m_speed * dt;
}

void Enemy::takeDamage(int amount) {
    m_lives -= amount;
    if (m_lives <= 0) {
        m_lives = 0;
        m_isAlive = false;
    }
}

glm::vec2 Enemy::getPosition() const {
    return m_position;
}

glm::vec2 Enemy::getSize() const {
    return m_size;
}

int Enemy::getLaneId() const {
    return m_laneId;
}

int Enemy::getLives() const {
    return m_lives;
}

int Enemy::getMaxLives() const {
    return m_maxLives;
}

bool Enemy::isAlive() const {
    return m_isAlive;
}

EnemyType Enemy::getType() const {
    return m_type;
}

const std::vector<float>& Enemy::getVertices() const {
    return m_vertices;
}

const std::vector<unsigned int>& Enemy::getIndices() const {
    return m_indices;
}