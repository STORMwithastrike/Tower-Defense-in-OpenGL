#include "../headers/HomeBase.h"

HomeBase::HomeBase(float startX, float startY, float width, float height)
	: m_position(startX, startY), m_size(width, height), m_isBreached(false) {

}

bool HomeBase::checkEnemyReached(float enemyX) {

	if (enemyX <= (m_position.x + m_size.x)) {
		m_isBreached = true;
		return true;
	}
	return false;
}

glm::vec2 HomeBase::getPosition() const {
	return m_position;
}

glm::vec2 HomeBase::getSize() const {
	return m_size;
}

bool HomeBase::hasLostGame() const {
	return m_isBreached;
}