#include "../headers/HomeBase.h"

HomeBase::HomeBase(float startX, float startY, float width, float height, int maxHp)
	: m_position(startX, startY),
	  m_size(width, height),
	  m_isBreached(false),
	  m_currentHealth(maxHp),
	  m_maxHealth(maxHp)
{
}

bool HomeBase::checkEnemyReached(float enemyX) {
	if (enemyX <= (m_position.x + m_size.x)) {
		takeDamage(3); // Direct physical enemy breach causes heavy damage
		return true;
	}
	return false;
}

void HomeBase::takeDamage(int amount) {
	m_currentHealth -= amount;
	if (m_currentHealth <= 0) {
		m_currentHealth = 0;
		m_isBreached = true;
	}
}

void HomeBase::reset() {
	m_currentHealth = m_maxHealth;
	m_isBreached = false;
}

glm::vec2 HomeBase::getPosition() const {
	return m_position;
}

glm::vec2 HomeBase::getSize() const {
	return m_size;
}

bool HomeBase::hasLostGame() const {
	return m_isBreached || m_currentHealth <= 0;
}
