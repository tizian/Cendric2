#include "Level/MOBBehavior/MovingBehaviors/AllyWalkingBehavior.h"

AllyWalkingBehavior::AllyWalkingBehavior(Enemy* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy),
	WalkingBehavior(enemy) {
	m_replaceDistance = 600.f;
}

void AllyWalkingBehavior::checkCollisions(const sf::Vector2f& nextPosition) {
	WalkingBehavior::checkCollisions(nextPosition);
	// if the enemy collidesX but can't jump and is ally and idle, teleports to its owner.
	EnemyState state = m_enemy->getEnemyState();
	if (state != EnemyState::Chasing && !m_jumps && m_mainChar->getMovingBehavior()->isGrounded() && dist(m_mainChar->getPosition(), m_enemy->getPosition()) > m_replaceDistance) {
		sf::Vector2f newPos(m_mainChar->getPosition().x, m_mainChar->getPosition().y + m_mainChar->getBoundingBox()->height - m_enemy->getBoundingBox()->height);
		WorldCollisionQueryRecord rec;
		rec.ignoreDynamicTiles = m_ignoreDynamicTiles;
		rec.boundingBox = *m_enemy->getBoundingBox();
		rec.boundingBox.left = newPos.x;
		rec.boundingBox.top = newPos.y;
		if (m_enemy->getLevel()->collides(rec)) return;
		m_enemy->setPosition(newPos);
		m_isCollisionTiltSuppressed = true;
	}
}

void AllyWalkingBehavior::setReplaceDistance(float replaceDistance) {
	m_replaceDistance = replaceDistance;
}

void AllyWalkingBehavior::execHandleMovementInput() {
	// movement AI
	bool hasTarget = m_enemy->getCurrentTarget() != nullptr;
	sf::Vector2f center = m_enemy->getCenter();
	sf::Vector2f targetCenter = hasTarget ? m_enemy->getCurrentTarget()->getCenter() : center;
	if (hasTarget && (m_enemy->getEnemyState() == EnemyState::Chasing || m_enemy->getEnemyState() == EnemyState::Recovering)) {

		float approachingDistance = (std::abs(m_enemy->getVelocity().x) > 0.f) ? (m_approachingDistance - 5.f) : m_approachingDistance;

		if (targetCenter.x < center.x && std::abs(targetCenter.x - center.x) > approachingDistance) {
			m_movingDirectionX = -1;
		}
		else if (targetCenter.x > center.x && std::abs(targetCenter.x - center.x) > approachingDistance) {
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

		sf::Vector2f mainCharCenter = m_mainChar->getCenter();

		float approachingDistance = (std::abs(m_enemy->getVelocity().x) > 0.f) ? (m_approachingDistance - 5.f) : m_approachingDistance;

		if (mainCharCenter.x < center.x && std::abs(mainCharCenter.x - center.x) > approachingDistance) {
			m_movingDirectionX = -1;
		}
		else if (mainCharCenter.x > center.x && std::abs(mainCharCenter.x - center.x) > approachingDistance) {
			m_movingDirectionX = 1;
		}
		else {
			m_movingDirectionX = 0;
		}
	}
}

