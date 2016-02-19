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
	if (hasTarget && (m_enemy->getEnemyState() == EnemyState::Chasing || m_enemy->getEnemyState() == EnemyState::Recovering)) {

		if (targetCenter.x < center.x && std::abs(targetCenter.x - center.x) > m_approachingDistance) {
			m_nextIsFacingRight = false;
			newAccelerationX -= m_walkAcceleration;
		}
		else if (targetCenter.x > center.x && std::abs(targetCenter.x - center.x) > m_approachingDistance) {
			m_nextIsFacingRight = true;
			newAccelerationX += m_walkAcceleration;
		}

		if (m_jumps && m_isGrounded) {
			m_enemy->setVelocityY(m_isFlippedGravity ? m_configuredMaxVelocityYUp : -m_configuredMaxVelocityYUp); // first jump vel will always be max y vel. 
			m_jumps = false;
		}
	}
	else if (hasTarget && m_enemy->getEnemyState() == EnemyState::Fleeing) {

		if (targetCenter.x < center.x) {
			m_nextIsFacingRight = true;
			newAccelerationX += m_walkAcceleration;
		}
		else if (targetCenter.x > center.x) {
			m_nextIsFacingRight = false;
			newAccelerationX -= m_walkAcceleration;
		}

		if (m_jumps && m_isGrounded) {
			m_enemy->setVelocityY(-m_enemy->getConfiguredMaxVelocityYUp()); // first jump vel will always be max y vel. 
			m_jumps = false;
		}
	}
	else if (m_enemy->getEnemyState() == EnemyState::Idle || m_enemy->getEnemyState() == EnemyState::Waiting) {

		if (m_randomDecision != 0) {
			m_nextIsFacingRight = m_randomDecision == 1;
			newAccelerationX += m_randomDecision * m_walkAcceleration;
		}

		if (m_jumps && m_isGrounded) {
			m_enemy->setVelocityY(-m_enemy->getConfiguredMaxVelocityYUp()); // first jump vel will always be max y vel. 
			m_jumps = false;
		}
	}

	m_enemy->setAcceleration(sf::Vector2f(newAccelerationX, (m_isFlippedGravity ? -m_gravity : m_gravity)));
}

