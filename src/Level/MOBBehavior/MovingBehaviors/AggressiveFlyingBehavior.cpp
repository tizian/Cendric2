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
		sf::Vector2f center = m_enemy->getCenter();
		sf::Vector2f targetCenter = hasTarget ? m_enemy->getCurrentTarget()->getCenter() : center;

		if (targetCenter.x < center.x && std::abs(targetCenter.x - center.x) > m_approachingDistance) {
			m_movingDirectionX = -1;
		}
		else if (targetCenter.x > center.x && std::abs(targetCenter.x - center.x) > m_approachingDistance) {
			m_movingDirectionX = 1;
		}
		else {
			m_movingDirectionX = 0;
		}

		if (targetCenter.y < center.y && std::abs(targetCenter.y - center.y) > m_approachingDistance) {
			m_movingDirectionY = -1;
		}
		else if (targetCenter.y > center.y && std::abs(targetCenter.y - center.y) > m_approachingDistance) {
			m_movingDirectionY = 1;
		}
		else {
			m_movingDirectionY = 0;
		}
	}
	else if (hasTarget && m_enemy->getEnemyState() == EnemyState::Fleeing) {
		sf::Vector2f center = m_enemy->getCenter();
		sf::Vector2f targetCenter = hasTarget ? m_enemy->getCurrentTarget()->getCenter() : center;

		m_movingDirectionX = (targetCenter.x < center.x) ? 1 : -1;
		m_movingDirectionY = (targetCenter.y < center.y) ? 1 : -1;
	}
}

