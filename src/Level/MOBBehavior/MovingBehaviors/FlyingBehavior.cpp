#include "Level/MOBBehavior/MovingBehaviors/FlyingBehavior.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"

FlyingBehavior::FlyingBehavior(Enemy* enemy) : 
	EnemyMovingBehavior(enemy),
	MovingBehavior(enemy) {
	m_avoidableTiles.insert(LevelDynamicTileID::Water);
	m_avoidableTiles.insert(LevelDynamicTileID::SpikesTop);
	m_avoidableTiles.insert(LevelDynamicTileID::SpikesBottom);
};

void FlyingBehavior::makeRandomDecision() {
	m_randomDecisionX = rand() % 3 - 1;
	m_randomDecisionY = rand() % 3 - 1;
}

void FlyingBehavior::checkCollisions(const sf::Vector2f& nextPosition) {
	bool collidesX = false;
	bool collidesY = false;

	const sf::FloatRect& bb = *m_mob->getBoundingBox();
	const Level& level = *m_mob->getLevel();
	sf::FloatRect nextBoundingBoxX(nextPosition.x, bb.top, bb.width, bb.height);
	sf::FloatRect nextBoundingBoxY(bb.left, nextPosition.y, bb.width, bb.height);
	WorldCollisionQueryRecord rec;
	rec.ignoreDynamicTiles = m_ignoreDynamicTiles;

	bool isMovingDown = nextPosition.y > bb.top;
	bool isMovingRight = nextPosition.x > bb.left;

	// should we use strategy 2: try y direction first, then x direction?
	bool tryYfirst = false;
	rec.boundingBox = nextBoundingBoxX;
	rec.collisionDirection = isMovingRight ? CollisionDirection::Right : CollisionDirection::Left;
	if (level.collides(rec)) {
		if (std::abs(nextPosition.x - rec.safeLeft) > m_mob->getVelocity().x + 10.f) {
			tryYfirst = true;
		}
	}

	if (!tryYfirst) {

		// check for collision on x axis
		rec.boundingBox = nextBoundingBoxX;
		rec.collisionDirection = isMovingRight ? CollisionDirection::Right : CollisionDirection::Left;
		collidesX = level.collides(rec);
		if (collidesX) {
			m_mob->setAccelerationX(0.f);
			m_mob->setVelocityX(0.f);
			m_mob->setPositionX(rec.safeLeft);
			nextBoundingBoxY.left = rec.safeLeft;
		}
		else {
			nextBoundingBoxY.left = nextPosition.x;
		}

		// check for collision on y axis
		rec.boundingBox = nextBoundingBoxY;
		rec.collisionDirection = isMovingDown ? CollisionDirection::Down : CollisionDirection::Up;
		collidesY = level.collides(rec);
		if (collidesY) {
			m_mob->setAccelerationY(0.f);
			m_mob->setVelocityY(0.f);
			m_mob->setPositionY(rec.safeTop);
		}
	}
	else {
		// check for collision on y axis
		rec.boundingBox = nextBoundingBoxY;
		rec.collisionDirection = isMovingDown ? CollisionDirection::Down : CollisionDirection::Up;
		bool isFalling = isUpsideDown() != isMovingDown;
		collidesY = level.collides(rec);
		if (collidesY) {
			m_mob->setAccelerationY(0.f);
			m_mob->setVelocityY(0.f);
			m_mob->setPositionY(rec.safeTop);
			nextBoundingBoxX.top = rec.safeTop;
		}
		else {
			nextBoundingBoxX.top = nextPosition.y;
		}

		// check for collision on x axis
		rec.boundingBox = nextBoundingBoxX;
		rec.collisionDirection = isMovingRight ? CollisionDirection::Right : CollisionDirection::Left;

		collidesX = level.collides(rec);
		if (collidesX) {
			m_mob->setAccelerationX(0.f);
			m_mob->setVelocityX(0.f);
			m_mob->setPositionX(rec.safeLeft);
		}
	}
}

void FlyingBehavior::handleMovementInput() {
	if (m_enemy->isDead()) {
		m_enemy->setAcceleration(sf::Vector2f(0, getGravity()));
		return;
	}
	// movement AI
	float newAccelerationX = m_mob->getAcceleration().x;
	float newAccelerationY = 0.f;
	bool hasTarget = m_enemy->getCurrentTarget() != nullptr;
	sf::Vector2f center = m_enemy->getCenter();
	sf::Vector2f targetCenter = hasTarget ? m_enemy->getCurrentTarget()->getCenter() : center;

	if (hasTarget && (m_enemy->getEnemyState() == EnemyState::Chasing || m_enemy->getEnemyState() == EnemyState::Recovering)) {

		if (targetCenter.x < center.x && std::abs(targetCenter.x - center.x) > m_approachingDistance) {
			m_nextIsFacingRight = false;
			newAccelerationX -= m_walkAcceleration;
		}
		else if (targetCenter.x > center.x && std::abs(targetCenter.x - center.x) > m_approachingDistance) {
			m_nextIsFacingRight = true;
			newAccelerationX += m_walkAcceleration;
		}

		if (targetCenter.y < center.y && std::abs(targetCenter.y - center.y) > m_approachingDistance) {
			newAccelerationY -= m_walkAcceleration;
		}
		else if (targetCenter.y > center.y && std::abs(targetCenter.y - center.y) > m_approachingDistance) {
			newAccelerationY += m_walkAcceleration;
		}
	}
	else if (hasTarget && m_enemy->getEnemyState() == EnemyState::Fleeing) {

		if (targetCenter.x < center.x) {
			m_nextIsFacingRight = true;
			newAccelerationX += m_walkAcceleration;
		}
		else if (targetCenter.x > center.x) {
			m_nextIsFacingRight = false;
			newAccelerationX -= m_walkAcceleration;
		}

		if (targetCenter.y < center.y) {
			newAccelerationY += m_walkAcceleration;
		}
		else if (targetCenter.y > center.y) {
			newAccelerationY -= m_walkAcceleration;
		}
	}
	else if (m_enemy->getEnemyState() == EnemyState::Idle || m_enemy->getEnemyState() == EnemyState::Waiting) {
		if (m_enemy->isAlly()) {
			sf::Vector2f mainCharCenter = m_mainChar->getCenter();
			if (mainCharCenter.x < center.x && std::abs(mainCharCenter.x - center.x) > m_approachingDistance) {
				m_nextIsFacingRight = false;
				newAccelerationX -= m_walkAcceleration;
			}
			else if (mainCharCenter.x > center.x && std::abs(mainCharCenter.x - center.x) > m_approachingDistance) {
				m_nextIsFacingRight = true;
				newAccelerationX += m_walkAcceleration;
			}

			if (mainCharCenter.y < center.y && std::abs(mainCharCenter.y - center.y) > 2 * m_approachingDistance) {
				newAccelerationY -= m_walkAcceleration;
			}
			else if (mainCharCenter.y > center.y && std::abs(mainCharCenter.y - center.y) > 2 * m_approachingDistance) {
				newAccelerationY += m_walkAcceleration;
			}
		}
		else {
			if (m_randomDecisionX != 0) {
				m_nextIsFacingRight = (m_randomDecisionX == 1);
				newAccelerationX += (m_randomDecisionX * m_walkAcceleration);
			}
			if (m_randomDecisionY != 0) {
				newAccelerationY += (m_randomDecisionY * m_walkAcceleration);
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
	else if (m_fightAnimationTime > sf::Time::Zero) {
		newState = GameObjectState::Fighting;
	}
	else if (std::abs(m_enemy->getVelocity().x) > 20.f || std::abs(m_enemy->getVelocity().y) > 20.f) {
		newState = GameObjectState::Flying;
	}

	// only update animation if we need to
	if (m_enemy->getState() != newState || m_nextIsFacingRight != m_isFacingRight) {
		m_isFacingRight = m_nextIsFacingRight;
		m_enemy->setState(newState);
		m_enemy->setCurrentAnimation(m_enemy->getAnimation(m_enemy->getState()), !m_isFacingRight);
	}
}

