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
	else if (m_enemy->getEnemyState() == EnemyState::Idle || m_enemy->getEnemyState() == EnemyState::Waiting) {
		
			sf::Vector2f mainCharCenter = m_mainChar->getCenter();
			if (mainCharCenter.x < center.x && std::abs(mainCharCenter.x - center.x) > m_approachingDistance) {
				m_movingDirectionX = -1;
			}
			else if (mainCharCenter.x > center.x && std::abs(mainCharCenter.x - center.x) > m_approachingDistance) {
				m_movingDirectionX = 1;
			}
			else {
				m_movingDirectionX = 0;
			}

			if (mainCharCenter.y < center.y && std::abs(mainCharCenter.y - center.y) > 2 * m_approachingDistance) {
				m_movingDirectionY = -1;
			}
			else if (mainCharCenter.y > center.y && std::abs(mainCharCenter.y - center.y) > 2 * m_approachingDistance) {
				m_movingDirectionY = 1;
			}
			else {
				m_movingDirectionY = 0;
			}
	}
}

