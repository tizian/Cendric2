#include "Level/MOBBehavior/MovingBehaviors/EnemyMovingBehavior.h"

EnemyMovingBehavior::EnemyMovingBehavior(Enemy* enemy) : MovingBehavior(enemy) {
	m_enemy = enemy;
}

EnemyMovingBehavior::~EnemyMovingBehavior() {
	delete m_movingTarget;
}

void EnemyMovingBehavior::handleMovementInput() {
	if (m_enemy->isDead()) {
		m_enemy->setAcceleration(sf::Vector2f(0.f, getGravity()));
		return;
	}

	if (!m_enemy->isStunned()) {
		if (m_movingTarget != nullptr) {
			sf::Vector2f center = m_enemy->getCenter();
			// the enemy tries to get near its target
			if (m_movingTarget->x < center.x && std::abs(m_movingTarget->x - center.x) > 10.f) {
				m_movingDirectionX = -1;
			}
			else if (m_movingTarget->x > center.x && std::abs(m_movingTarget->x - center.x) > 10.f) {
				m_movingDirectionX = 1;
			}
			else {
				m_movingDirectionX = 0;
			}

			if (m_movingTarget->y < center.y && std::abs(m_movingTarget->y - center.y) > 10.f) {
				m_movingDirectionY = -1;
			}
			else if (m_movingTarget->y > center.x && std::abs(m_movingTarget->y - center.y) > 10.f) {
				m_movingDirectionY = 1;
			}
			else {
				m_movingDirectionY = 0;
			}
		}
		else {
			execHandleMovementInput();
		}
	}

	float newAccelerationX = m_enemy->getAcceleration().x;
	m_nextIsFacingRight = (m_movingDirectionX == 0) ? m_nextIsFacingRight : (m_movingDirectionX == 1);

	if (!m_enemy->isStunned()) {
		newAccelerationX += m_movingDirectionX * m_walkAcceleration;
	}

	float newAccelerationY = m_isWalkingBehavior ? 
		(m_isFlippedGravity ? -m_gravity : m_gravity) :
		m_movingDirectionY * m_walkAcceleration;

	m_enemy->setAcceleration(sf::Vector2f(newAccelerationX, newAccelerationY));
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
}
