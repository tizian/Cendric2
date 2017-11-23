#include "Level/MOBBehavior/MovingBehaviors/EnemyMovingBehavior.h"

EnemyMovingBehavior::EnemyMovingBehavior(Enemy* enemy) : MovingBehavior(enemy) {
	m_enemy = enemy;
}

EnemyMovingBehavior::~EnemyMovingBehavior() {
	delete m_movingTarget;
}

void EnemyMovingBehavior::setFacingRight(bool value) {
	MovingBehavior::setFacingRight(value);
	m_movingDirectionX = value ? 1 : -1;
}

void EnemyMovingBehavior::handleMovementInput() {
	if (m_movingTarget != nullptr) {
		gotoTarget(*m_movingTarget, getMaxVelocityX() * MAX_FRAME_TIME, true, true);
	}
	else {
		execHandleMovementInput();
	}

	handleTrueAcceleration();
}

void EnemyMovingBehavior::resetMovementLock() {
	m_isMovingXLocked = false;
	m_isMovingYLocked = false;
}

void EnemyMovingBehavior::gotoTarget(const sf::Vector2f& target, float approachingDistance, bool hold, bool exact) {
	sf::Vector2f center = m_enemy->getCenter();

	// the enemy tries to get near its target
	if (!m_isMovingXLocked) {
		if (target.x < center.x && std::abs(target.x - center.x) > approachingDistance) {
			m_movingDirectionX = -1;
		}
		else if (target.x > center.x && std::abs(target.x - center.x) > approachingDistance) {
			m_movingDirectionX = 1;
		}
		else {
			m_movingDirectionX = 0;
			if (hold) {
				m_enemy->setVelocity(sf::Vector2f(0.f, m_enemy->getVelocity().y));
				m_enemy->setAccelerationX(0.f);
			}
			if (exact) {
				m_enemy->setPositionX(target.x - (center.x - m_enemy->getPosition().x));
			}
		}
	}

	if (!m_isMovingYLocked) {
		if (target.y < center.y && std::abs(target.y - center.y) > approachingDistance) {
			m_movingDirectionY = -1;
		}
		else if (target.y > center.y && std::abs(target.y - center.y) > approachingDistance) {
			m_movingDirectionY = 1;
		}
		else {
			m_movingDirectionY = 0;
			if (hold && !m_isWalkingBehavior) {
				m_enemy->setVelocity(sf::Vector2f(m_enemy->getVelocity().x, 0.f));
				m_enemy->setAccelerationY(0.f);
			}
			if (exact && !m_isWalkingBehavior) {
				m_enemy->setPositionY(target.y - (center.y - m_enemy->getPosition().y));
			}
		}
	}

	// handle locks
	if (m_isWalkingBehavior) return;

	if (m_isCollidingY && !m_isMovingXLocked) {
		if (m_isCollidingX) {
			m_movingDirectionX = m_movingDirectionX == 1 ? -1 : 1;
		}
		else {
			m_movingDirectionX = m_movingDirectionX = rand() % 2  == 0 ? -1 : 1;
		}
		m_isMovingXLocked = true;
		return;
	}

	if (m_isCollidingX && !m_isMovingYLocked) {
		if (m_isCollidingY) {
			m_movingDirectionY = m_movingDirectionY == 1 ? -1 : 1;
		}
		else {
			m_movingDirectionY = m_movingDirectionY = rand() % 2 == 0 ? -1 : 1;
		}
		m_isMovingYLocked = true;
		return;
	} 

	if (!m_isCollidingX && m_isMovingYLocked) {
		m_isMovingYLocked = false;
	}

	if (!m_isCollidingY && m_isMovingXLocked) {
		m_isMovingXLocked = false;
	}
}

void EnemyMovingBehavior::setApproachingDistance(float distance) {
	m_approachingDistance = distance;
}

void EnemyMovingBehavior::setMovingTarget(int x, int y) {
	delete m_movingTarget;
	m_movingTarget = new sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
}

void EnemyMovingBehavior::resetMovingTarget() {
	delete m_movingTarget;
	m_movingTarget = nullptr;
	m_movingDirectionX = 0;
	m_movingDirectionY = 0;
	m_enemy->setVelocity(sf::Vector2f(0.f, 0.f));
}
