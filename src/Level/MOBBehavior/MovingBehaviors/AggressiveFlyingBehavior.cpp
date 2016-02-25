#include "Level/MOBBehavior/MovingBehaviors/AggressiveFlyingBehavior.h"

AggressiveFlyingBehavior::AggressiveFlyingBehavior(Enemy* enemy) :
	EnemyMovingBehavior(enemy),
	MovingBehavior(enemy),
	FlyingBehavior(enemy) {
}

void AggressiveFlyingBehavior::execHandleMovementInput() {

	// movement AI
	float newAccelerationX = m_mob->getAcceleration().x;
	float newAccelerationY = 0.f;
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

		if (targetCenter.y < center.y && std::abs(targetCenter.y - center.y) > m_approachingDistance) {
			newAccelerationY -= m_walkAcceleration;
		}
		else if (targetCenter.y > center.y && std::abs(targetCenter.y - center.y) > m_approachingDistance) {
			newAccelerationY += m_walkAcceleration;
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

		if (targetCenter.y < center.y) {
			newAccelerationY += m_walkAcceleration;
		}
		else if (targetCenter.y > center.y) {
			newAccelerationY -= m_walkAcceleration;
		}
	}
	else if (m_enemy->getEnemyState() == EnemyState::Idle || m_enemy->getEnemyState() == EnemyState::Waiting) {

		if (m_movingDirectionX != 0) {
			m_nextIsFacingRight = (m_movingDirectionX == 1);
			newAccelerationX += (m_movingDirectionX * m_walkAcceleration);
		}
		if (m_movingDirectionY != 0) {
			newAccelerationY += (m_movingDirectionY * m_walkAcceleration);
		}
	}

	m_enemy->setAcceleration(sf::Vector2f(newAccelerationX, newAccelerationY));
}

