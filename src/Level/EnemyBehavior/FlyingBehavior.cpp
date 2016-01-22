#include "Level/EnemyBehavior/FlyingBehavior.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"

FlyingBehavior::FlyingBehavior(Enemy* enemy) : MovingBehavior(enemy) {
	m_avoidableTiles.insert(LevelDynamicTileID::Water);
	m_avoidableTiles.insert(LevelDynamicTileID::SpikesTop);
	m_avoidableTiles.insert(LevelDynamicTileID::SpikesBottom);
};

void FlyingBehavior::makeRandomDecision() {
	m_randomDecisionX = rand() % 3 - 1;
	m_randomDecisionY = rand() % 3 - 1;
}

void FlyingBehavior::checkCollisions(const sf::Vector2f& nextPosition) {
	const sf::FloatRect bb = *m_enemy->getBoundingBox();
	const Level& level = *m_enemy->getLevel();
	sf::FloatRect nextBoundingBoxX(nextPosition.x, bb.top, bb.width, bb.height);
	sf::FloatRect nextBoundingBoxY(bb.left, nextPosition.y, bb.width, bb.height);

	bool isMovingDown = nextPosition.y > bb.top; // the mob is always moving either up or down, because of gravity. There are very, very rare, nearly impossible cases where they just cancel out.
	bool isMovingX = nextPosition.x != bb.left;
	bool isMovingRight = nextPosition.x > bb.left;

	// check for collision on x axis
	bool collidesX = false;
	if (isMovingX &&
		(level.collides(nextBoundingBoxX, m_enemy->m_ignoreDynamicTiles) ||
			level.collidesWithDynamicTiles(&nextBoundingBoxX, m_avoidableTiles))) {
		m_enemy->setAccelerationX(0.0f);
		m_enemy->setVelocityX(0.0f);
		m_randomDecisionX = isMovingRight ? -1 : 1;
	}
	// check for collision on y axis
	if (level.collides(nextBoundingBoxY, m_enemy->m_ignoreDynamicTiles) ||
		level.collidesWithDynamicTiles(&nextBoundingBoxY, m_avoidableTiles)) {
		m_enemy->setAccelerationY(0.0);
		m_enemy->setVelocityY(0.0f);
		m_randomDecisionY = isMovingDown ? -1 : 1;
	}
}

void FlyingBehavior::handleMovementInput() {
	// movement AI
	float newAccelerationX = m_enemy->getAcceleration().x;
	float newAccelerationY = 0.f;
	bool hasTarget = m_enemy->getCurrentTarget() != nullptr;
	sf::Vector2f center = m_enemy->getCenter();
	sf::Vector2f targetCenter = hasTarget ? m_enemy->getCurrentTarget()->getCenter() : center;

	if (hasTarget && (m_enemy->m_enemyState == EnemyState::Chasing || m_enemy->m_enemyState == EnemyState::Recovering)) {

		if (targetCenter.x < center.x && std::abs(targetCenter.x - center.x) > getApproachingDistance()) {
			m_enemy->m_nextIsFacingRight = false;
			newAccelerationX -= m_enemy->getConfiguredWalkAcceleration();
		}
		else if (targetCenter.x > center.x && std::abs(targetCenter.x - center.x) > getApproachingDistance()) {
			m_enemy->m_nextIsFacingRight = true;
			newAccelerationX += m_enemy->getConfiguredWalkAcceleration();
		}

		if (targetCenter.y < center.y && std::abs(targetCenter.y - center.y) > getApproachingDistance()) {
			newAccelerationY -= m_enemy->getConfiguredWalkAcceleration();
		}
		else if (targetCenter.y > center.y && std::abs(targetCenter.y - center.y) > getApproachingDistance()) {
			newAccelerationY += m_enemy->getConfiguredWalkAcceleration();
		}
	}
	else if (hasTarget && m_enemy->m_enemyState == EnemyState::Fleeing) {

		if (targetCenter.x < center.x) {
			m_enemy->m_nextIsFacingRight = true;
			newAccelerationX += m_enemy->getConfiguredWalkAcceleration();
		}
		else if (targetCenter.x > center.x) {
			m_enemy->m_nextIsFacingRight = false;
			newAccelerationX -= m_enemy->getConfiguredWalkAcceleration();
		}

		if (targetCenter.y < center.y) {
			newAccelerationY += m_enemy->getConfiguredWalkAcceleration();
		}
		else if (targetCenter.y > center.y) {
			newAccelerationY -= m_enemy->getConfiguredWalkAcceleration();
		}
	}
	else if (m_enemy->m_enemyState == EnemyState::Idle || m_enemy->m_enemyState == EnemyState::Waiting) {
		if (m_enemy->isAlly()) {
			sf::Vector2f mainCharCenter = m_enemy->m_mainChar->getCenter();
			if (mainCharCenter.x < center.x && std::abs(mainCharCenter.x - center.x) > 2 * getApproachingDistance()) {
				m_enemy->m_nextIsFacingRight = false;
				newAccelerationX -= m_enemy->getConfiguredWalkAcceleration();
			}
			else if (mainCharCenter.x > center.x && std::abs(mainCharCenter.x - center.x) > 2 * getApproachingDistance()) {
				m_enemy->m_nextIsFacingRight = true;
				newAccelerationX += m_enemy->getConfiguredWalkAcceleration();
			}

			if (mainCharCenter.y < center.y && std::abs(mainCharCenter.y - center.y) > 2 * getApproachingDistance()) {
				newAccelerationY -= m_enemy->getConfiguredWalkAcceleration();
			}
			else if (mainCharCenter.y > center.y && std::abs(mainCharCenter.y - center.y) > 2 * getApproachingDistance()) {
				newAccelerationY += m_enemy->getConfiguredWalkAcceleration();
			}
		}
		else {
			if (m_randomDecisionX != 0) {
				m_enemy->m_nextIsFacingRight = (m_randomDecisionX == 1);
				newAccelerationX += (m_randomDecisionX * m_enemy->getConfiguredWalkAcceleration());
			}
			if (m_randomDecisionY != 0) {
				newAccelerationY += (m_randomDecisionY * m_enemy->getConfiguredWalkAcceleration());
			}
		}
	}

	m_enemy->setAcceleration(sf::Vector2f(newAccelerationX, newAccelerationY));
}

void FlyingBehavior::updateAnimation() {
	// calculate new game state and set animation.

	GameObjectState newState = GameObjectState::Idle;
	if (m_enemy->isDead()) {
		newState = GameObjectState::Dead;
	}
	else if (m_enemy->m_fightAnimationTime > sf::Time::Zero) {
		newState = GameObjectState::Fighting;
	}
	else if (std::abs(m_enemy->getVelocity().x) > 20.f || std::abs(m_enemy->getVelocity().y) > 20.f) {
		newState = GameObjectState::Flying;
	}

	// only update animation if we need to
	if (m_enemy->m_state != newState || m_enemy->m_nextIsFacingRight != m_enemy->m_isFacingRight) {
		m_enemy->m_isFacingRight = m_enemy->m_nextIsFacingRight;
		m_enemy->m_state = newState;
		m_enemy->setCurrentAnimation(m_enemy->getAnimation(m_enemy->m_state), !m_enemy->m_isFacingRight);
	}
}

