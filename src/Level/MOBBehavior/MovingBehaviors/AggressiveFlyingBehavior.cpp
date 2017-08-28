#include "Level/MOBBehavior/MovingBehaviors/AggressiveFlyingBehavior.h"

AggressiveFlyingBehavior::AggressiveFlyingBehavior(Enemy* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	FlyingBehavior(enemy) {
}

void AggressiveFlyingBehavior::execHandleMovementInput() {
	// movement AI
	bool hasTarget = m_enemy->getCurrentTarget() != nullptr;
	if (hasTarget && m_enemy->getEnemyState() == EnemyState::Chasing) {
		sf::Vector2f targetCenter = m_enemy->getCurrentTarget()->getCenter();

		gotoTarget(targetCenter, m_approachingDistance);
	}
	else if (hasTarget && m_enemy->getEnemyState() == EnemyState::Fleeing) {
		sf::Vector2f center = m_enemy->getCenter();
		sf::Vector2f targetCenter = hasTarget ? m_enemy->getCurrentTarget()->getCenter() : center;

		m_movingDirectionX = (targetCenter.x < center.x) ? 1 : -1;
		m_movingDirectionY = (targetCenter.y < center.y) ? 1 : -1;
	}
}

