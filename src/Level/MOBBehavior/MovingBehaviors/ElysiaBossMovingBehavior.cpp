#include "Level/MOBBehavior/MovingBehaviors/ElysiaBossMovingBehavior.h"

ElysiaBossMovingBehavior::ElysiaBossMovingBehavior(Enemy* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	FlyingBehavior(enemy) {
}

void ElysiaBossMovingBehavior::execHandleMovementInput() {
	// TODO
	// movement AI
	if (m_enemy->getEnemyState() == EnemyState::Recovering)
		return;

	bool hasTarget = m_enemy->getCurrentTarget() != nullptr;
	sf::Vector2f center = m_enemy->getCenter();
	sf::Vector2f targetCenter = hasTarget ? m_enemy->getCurrentTarget()->getCenter() : center;

	if (hasTarget && m_enemy->getEnemyState() == EnemyState::Chasing) {

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
		m_movingDirectionX = (targetCenter.x < center.x) ? 1 : -1;
		m_movingDirectionY = (targetCenter.y < center.y) ? 1 : -1;
	}
}

void ElysiaBossMovingBehavior::update(const sf::Time& frameTime) {
	// TODO
	FlyingBehavior::update(frameTime);
}




