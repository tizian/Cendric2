#include "Level/MOBBehavior/MovingBehaviors/AllyFlyingBehavior.h"

AllyFlyingBehavior::AllyFlyingBehavior(Enemy* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	FlyingBehavior(enemy) {
}

void AllyFlyingBehavior::execHandleMovementInput() {
	// movement AI
	bool hasTarget = m_enemy->getCurrentTarget() != nullptr;
	sf::Vector2f center = m_enemy->getCenter();
	sf::Vector2f targetCenter = hasTarget ? m_enemy->getCurrentTarget()->getCenter() : center;

	if (hasTarget && (m_enemy->getEnemyState() == EnemyState::Chasing)) {
		gotoTarget(targetCenter, m_approachingDistance);
	}
	else if (hasTarget && m_enemy->getEnemyState() == EnemyState::Fleeing) {
		m_movingDirectionX = (targetCenter.x < center.x) ? 1 : -1;
		m_movingDirectionY = (targetCenter.y < center.y) ? 1 : -1;
	}
	else if (m_enemy->getEnemyState() == EnemyState::Idle || m_enemy->getEnemyState() == EnemyState::Waiting) {
		gotoTarget(m_mainChar->getCenter(), m_approachingDistance);
	}
}

