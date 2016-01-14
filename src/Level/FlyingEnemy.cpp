#include "Level/FlyingEnemy.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"

FlyingEnemy::FlyingEnemy(Level* level, Screen* screen, bool isControlled) : 
	Enemy(level, screen, isControlled), 
	LevelMovableGameObject(level) {
	m_avoidableTiles.insert(LevelDynamicTileID::Water);
	m_avoidableTiles.insert(LevelDynamicTileID::SpikesTop);
	m_avoidableTiles.insert(LevelDynamicTileID::SpikesBottom);
}

FlyingEnemy::~FlyingEnemy() {
}

void FlyingEnemy::checkCollisions(const sf::Vector2f& nextPosition) {
	sf::FloatRect nextBoundingBoxX(nextPosition.x, getBoundingBox()->top, getBoundingBox()->width, getBoundingBox()->height);
	sf::FloatRect nextBoundingBoxY(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);

	bool isMovingDown = nextPosition.y > getBoundingBox()->top; // the mob is always moving either up or down, because of gravity. There are very, very rare, nearly impossible cases where they just cancel out.
	bool isMovingX = nextPosition.x != getBoundingBox()->left;
	bool isMovingRight = nextPosition.x > getBoundingBox()->left;

	// check for collision on x axis
	bool collidesX = false;	
	if (isMovingX && 
		(m_level->collides(nextBoundingBoxX, m_ignoreDynamicTiles) || 
			m_level->collidesWithDynamicTiles(&nextBoundingBoxX, m_avoidableTiles))) {
		setAccelerationX(0.0f);
		setVelocityX(0.0f);
		m_randomDecisionX = isMovingRight ? -1 : 1;
	}
	// check for collision on y axis
	if (m_level->collides(nextBoundingBoxY, m_ignoreDynamicTiles) ||
		m_level->collidesWithDynamicTiles(&nextBoundingBoxY, m_avoidableTiles)) {
		setAccelerationY(0.0);
		setVelocityY(0.0f);
		m_randomDecisionY = isMovingDown ? -1 : 1;
	}
}

void FlyingEnemy::handleMovementInput() {
	if (m_isDead) {
		setAcceleration(sf::Vector2f(0.f, getConfiguredGravityAcceleration()));
		return;
	}

	// movement AI
	float newAccelerationX = 0.f;
	float newAccelerationY = 0.f;
	bool hasTarget = m_currentTarget != nullptr;
	sf::Vector2f center = getCenter();
	sf::Vector2f targetCenter = hasTarget ? m_currentTarget->getCenter() : center;

	if (hasTarget && (m_enemyState == EnemyState::Chasing || m_enemyState == EnemyState::Recovering)) {

		if (targetCenter.x < center.x && std::abs(targetCenter.x - center.x) > getApproachingDistance()) {
			m_nextIsFacingRight = false;
			newAccelerationX -= getConfiguredWalkAcceleration();
		}
		else if (targetCenter.x > center.x && std::abs(targetCenter.x - center.x) > getApproachingDistance()) {
			m_nextIsFacingRight = true;
			newAccelerationX += getConfiguredWalkAcceleration();
		}

		if (targetCenter.y < center.y && std::abs(targetCenter.y - center.y) > getApproachingDistance()) {
			newAccelerationY -= getConfiguredWalkAcceleration();
		}
		else if (targetCenter.y > center.y && std::abs(targetCenter.y - center.y) > getApproachingDistance()) {
			newAccelerationY += getConfiguredWalkAcceleration();
		}
	}
	else if (hasTarget && m_enemyState == EnemyState::Fleeing) {

		if (targetCenter.x < center.x) {
			m_nextIsFacingRight = true;
			newAccelerationX += getConfiguredWalkAcceleration();
		}
		else if (targetCenter.x > center.x) {
			m_nextIsFacingRight = false;
			newAccelerationX -= getConfiguredWalkAcceleration();
		}

		if (targetCenter.y < center.y) {
			newAccelerationY += getConfiguredWalkAcceleration();
		}
		else if (targetCenter.y > center.y) {
			newAccelerationY -= getConfiguredWalkAcceleration();
		}
	}
	else if (m_enemyState == EnemyState::Idle || m_enemyState == EnemyState::Waiting) {
		if (m_isControlled) {
			sf::Vector2f mainCharCenter = m_mainChar->getCenter();
			if (mainCharCenter.x < center.x && std::abs(mainCharCenter.x - center.x) > 2 * getApproachingDistance()) {
				m_nextIsFacingRight = false;
				newAccelerationX -= getConfiguredWalkAcceleration();
			}
			else if (mainCharCenter.x > center.x && std::abs(mainCharCenter.x - center.x) > 2 * getApproachingDistance()) {
				m_nextIsFacingRight = true;
				newAccelerationX += getConfiguredWalkAcceleration();
			}

			if (mainCharCenter.y < center.y && std::abs(mainCharCenter.y - center.y) > 2 * getApproachingDistance()) {
				newAccelerationY -= getConfiguredWalkAcceleration();
			}
			else if (mainCharCenter.y > center.y && std::abs(mainCharCenter.y - center.y) > 2 * getApproachingDistance()) {
				newAccelerationY += getConfiguredWalkAcceleration();
			}
		}
		else {
			if (m_randomDecisionX != 0) {
				m_nextIsFacingRight = (m_randomDecisionX == 1);
				newAccelerationX += (m_randomDecisionX * getConfiguredWalkAcceleration());
			}
			if (m_randomDecisionY != 0) {
				newAccelerationY += (m_randomDecisionY * getConfiguredWalkAcceleration());
			}
		}
	}

	setAcceleration(sf::Vector2f(newAccelerationX, newAccelerationY));
}

void FlyingEnemy::makeRandomDecision() {
	m_randomDecisionX = rand() % 3 - 1;
	m_randomDecisionY = rand() % 3 - 1;
}

void FlyingEnemy::updateAnimation(const sf::Time& frameTime) {
	// calculate new game state and set animation.

	GameObjectState newState = GameObjectState::Idle;
	if (m_isDead) {
		newState = GameObjectState::Dead;
	}
	else if (m_fightAnimationTime > sf::Time::Zero) {
		newState = GameObjectState::Fighting;
	}
	else if (std::abs(getVelocity().x) > 20.f || std::abs(getVelocity().y) > 20.f) {
		newState = GameObjectState::Flying;
	}

	// only update animation if we need to
	if (m_state != newState || m_nextIsFacingRight != m_isFacingRight) {
		m_isFacingRight = m_nextIsFacingRight;
		m_state = newState;
		setCurrentAnimation(getAnimation(m_state), !m_isFacingRight);
	}
}


