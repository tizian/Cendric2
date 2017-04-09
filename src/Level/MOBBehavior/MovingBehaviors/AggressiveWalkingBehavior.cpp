#include "Level/MOBBehavior/MovingBehaviors/AggressiveWalkingBehavior.h"

AggressiveWalkingBehavior::AggressiveWalkingBehavior(Enemy* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	WalkingBehavior(enemy) {
}

void AggressiveWalkingBehavior::execHandleMovementInput() {
	if (m_enemy->getEnemyState() == EnemyState::Recovering || m_walksBlindly || m_jumpsBlindly)
		return;
	// movement AI
	bool hasTarget = m_enemy->getCurrentTarget() != nullptr;
	sf::Vector2f center = m_enemy->getCenter();
	sf::Vector2f targetCenter = hasTarget ? m_enemy->getCurrentTarget()->getCenter() : center;
	if (m_isGrounded && hasTarget && m_enemy->getEnemyState() == EnemyState::Chasing) {
		float approachingDistance = (std::abs(m_enemy->getVelocity().x) > 0.f) ? (m_approachingDistance - 5.f) : m_approachingDistance;
		// the enemy tries to get near its target
		if (targetCenter.x < center.x && std::abs(targetCenter.x - center.x) > approachingDistance) {
			m_movingDirectionX = -1;
		}
		else if (targetCenter.x > center.x && std::abs(targetCenter.x - center.x) > approachingDistance) {
			m_movingDirectionX = 1;
		}
		else {
			m_movingDirectionX = 0;
		}
		m_movingDirectionY = targetCenter.y < center.y ? -1 : 1;
	}
	else if (m_isGrounded && hasTarget && m_enemy->getEnemyState() == EnemyState::Fleeing) {
		// the enemy tries to get away from its target
		m_movingDirectionX = targetCenter.x < center.x ? 1 : -1;
		m_movingDirectionY = targetCenter.y < center.y ? 1 : -1;
	}
}

