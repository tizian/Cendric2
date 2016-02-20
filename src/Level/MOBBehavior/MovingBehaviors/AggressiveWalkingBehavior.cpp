#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"

AggressiveWalkingBehavior::AggressiveWalkingBehavior(Enemy* enemy) :
	EnemyMovingBehavior(enemy),
	MovingBehavior(enemy),
	WalkingBehavior(enemy) {
}

void AggressiveWalkingBehavior::handleMovementInput() {
	if (m_enemy->isDead()) {
		m_enemy->setAcceleration(sf::Vector2f(0.f, getGravity()));
		return;
	}

	// movement AI
	float newAccelerationX = m_enemy->getAcceleration().x;

	bool hasTarget = m_enemy->getCurrentTarget() != nullptr;
	sf::Vector2f center = m_enemy->getCenter();
	sf::Vector2f targetCenter = hasTarget ? m_enemy->getCurrentTarget()->getCenter() : center;
	if (m_isGrounded && hasTarget && (m_enemy->getEnemyState() == EnemyState::Chasing || m_enemy->getEnemyState() == EnemyState::Recovering)) {
		// the enemy tries to get near its target
		if (targetCenter.x < center.x && std::abs(targetCenter.x - center.x) > m_approachingDistance) {
			m_walkingDirection = -1;
		}
		else if (targetCenter.x > center.x && std::abs(targetCenter.x - center.x) > m_approachingDistance) {
			m_walkingDirection = 1;
		}
		else {
			m_walkingDirection = 0;
		}
	}
	else if (m_isGrounded && hasTarget && m_enemy->getEnemyState() == EnemyState::Fleeing) {
		// the enemy tries to get away from its target
		m_walkingDirection = targetCenter.x < center.x ? 1 : -1;
	}

	if (m_walkingDirection != 0) {
		m_nextIsFacingRight = m_walkingDirection == 1;
		newAccelerationX += m_walkingDirection * m_walkAcceleration;
	}

	if (m_jumps && m_isGrounded) {
		m_enemy->setVelocityY(-m_enemy->getConfiguredMaxVelocityYUp()); // first jump vel will always be max y vel. 
		m_jumps = false;
	}

	m_enemy->setAcceleration(sf::Vector2f(newAccelerationX, (m_isFlippedGravity ? -m_gravity : m_gravity)));
}

