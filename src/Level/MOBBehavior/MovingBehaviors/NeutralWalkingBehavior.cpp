#include "Level/MOBBehavior/MovingBehaviors/NeutralWalkingBehavior.h"

NeutralWalkingBehavior::NeutralWalkingBehavior(Enemy* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	WalkingBehavior(enemy) {
}

void NeutralWalkingBehavior::execHandleMovementInput() {
	// movement AI
	bool hasTarget = m_enemy->getCurrentTarget() != nullptr;
	sf::Vector2f center = m_enemy->getCenter();
	sf::Vector2f targetCenter = hasTarget ? m_enemy->getCurrentTarget()->getCenter() : center;
	if (hasTarget && (m_enemy->getEnemyState() == EnemyState::Chasing)) {
		if (targetCenter.x < center.x && std::abs(targetCenter.x - center.x) > m_approachingDistance) {
			m_movingDirectionX = -1;
		}
		else if (targetCenter.x > center.x && std::abs(targetCenter.x - center.x) > m_approachingDistance) {
			m_movingDirectionX = 1;
		}
		else {
			m_movingDirectionX = 0;
		}
	}
	else if (hasTarget && m_enemy->getEnemyState() == EnemyState::Fleeing) {
		m_movingDirectionX = (targetCenter.x < center.x) ? 1 : -1;
	}
	else if (m_enemy->getEnemyState() == EnemyState::Idle || m_enemy->getEnemyState() == EnemyState::Waiting) {
		m_movingDirectionX = 0;
	}
}

