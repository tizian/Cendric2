#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"

AllyWalkingBehavior::AllyWalkingBehavior(Enemy* enemy) :
	EnemyMovingBehavior(enemy),
	MovingBehavior(enemy),
	WalkingBehavior(enemy) {
}

void AllyWalkingBehavior::checkCollisions(const sf::Vector2f& nextPosition) {
	WalkingBehavior::checkCollisions(nextPosition);
	// if the enemy collidesX but can't jump and is ally and idle, teleports to its owner.
	if (m_enemy->getEnemyState() == EnemyState::Idle && m_collidesX && !m_jumps && m_mainChar->getMovingBehavior()->isGrounded()) {
		m_enemy->setPosition(sf::Vector2f(m_mainChar->getPosition().x, m_mainChar->getPosition().y + m_mainChar->getBoundingBox()->height - m_enemy->getBoundingBox()->height));
		m_isCollisionTiltSuppressed = true;
	}
}

void AllyWalkingBehavior::handleMovementInput() {
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

		sf::Vector2f mainCharCenter = m_mainChar->getCenter();

		if (mainCharCenter.x < center.x && std::abs(mainCharCenter.x - center.x) > m_approachingDistance) {
			m_nextIsFacingRight = false;
			newAccelerationX -= m_walkAcceleration;
		}
		else if (mainCharCenter.x > center.x && std::abs(mainCharCenter.x - center.x) > m_approachingDistance) {
			m_nextIsFacingRight = true;
			newAccelerationX += m_walkAcceleration;
		}

		if (m_jumps && m_isGrounded) {
			m_enemy->setVelocityY(m_isFlippedGravity ? m_configuredMaxVelocityYUp : -m_configuredMaxVelocityYUp); // first jump vel will always be max y vel. 
			m_jumps = false;
		}
	}

	m_enemy->setAcceleration(sf::Vector2f(newAccelerationX, (m_isFlippedGravity ? -m_gravity : m_gravity)));
}

