#include "Level/MOBBehavior/MovingBehaviors/FlyingBehavior.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"

FlyingBehavior::FlyingBehavior(Enemy* enemy) : 
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy) {

	m_dampingAirPerS = 0.1f;
	m_dampingGroundPerS = 0.7f;
	m_isWalkingBehavior = false;
};

void FlyingBehavior::makeRandomDecision() {
	m_movingDirectionX = rand() % 3 - 1;
	m_movingDirectionY = rand() % 3 - 1;
}

void FlyingBehavior::handleDefaultAcceleration() {
	if (m_mob->isDead()) {
		m_mob->setAcceleration(sf::Vector2f(0.f, getGravity()));
		return;
	}

	float newAccelerationX = m_mob->getAcceleration().x;
	m_mob->setAcceleration(sf::Vector2f(newAccelerationX, 0.f));
}

void FlyingBehavior::handleTrueAcceleration() {
	float newAccelerationY = m_movingDirectionY * m_walkAcceleration;
	float newAccelerationX = m_enemy->getAcceleration().x + m_movingDirectionX * m_walkAcceleration;

	m_nextIsFacingRight = (m_movingDirectionX == 0) ? m_nextIsFacingRight : (m_movingDirectionX == 1);

	m_enemy->setAcceleration(sf::Vector2f(newAccelerationX, newAccelerationY));
}

void FlyingBehavior::checkCollisions(const sf::Vector2f& nextPosition) {
	bool collidesX = false;
	bool collidesY = false;

	const sf::FloatRect& bb = *m_mob->getBoundingBox();
	const Level& level = *m_mob->getLevel();
	sf::FloatRect nextBoundingBoxX(nextPosition.x, bb.top, bb.width, bb.height);
	sf::FloatRect nextBoundingBoxY(bb.left, nextPosition.y, bb.width, bb.height);
	sf::FloatRect nextBoundingBox(nextPosition.x , nextPosition.y, bb.width, bb.height);

	bool isMovingDown = nextPosition.y > bb.top;
	bool isMovingRight = nextPosition.x > bb.left;
	bool isMovingY = nextPosition.y != bb.top;
	bool isMovingX = nextPosition.x != bb.left;
	
	if (level.collidesWithAvoidableTiles(nextBoundingBox)) {
		m_mob->setAcceleration(sf::Vector2f(0.f, 0.f));
		m_mob->setVelocity(sf::Vector2f(0.f, 0.f));
		if (isMovingX) {
			m_movingDirectionX = isMovingRight ? -1 : 1;
		}
		if (isMovingY) {
			m_movingDirectionY = isMovingDown ? -1 : 1;
		}
		return;
	}

	WorldCollisionQueryRecord rec;
	rec.ignoreOnewayTiles = true;
	rec.ignoreDynamicTiles = m_ignoreDynamicTiles;

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
			m_movingDirectionX = isMovingRight ? -1 : 1;
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
			m_movingDirectionY = isMovingDown ? -1 : 1;
		}
	}
	else {
		// check for collision on y axis
		rec.boundingBox = nextBoundingBoxY;
		rec.collisionDirection = isMovingDown ? CollisionDirection::Down : CollisionDirection::Up;
		collidesY = level.collides(rec);
		if (collidesY) {
			m_mob->setAccelerationY(0.f);
			m_mob->setVelocityY(0.f);
			m_mob->setPositionY(rec.safeTop);
			nextBoundingBoxX.top = rec.safeTop;
			m_movingDirectionY = isMovingDown ? -1 : 1;
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
			m_movingDirectionX = isMovingRight ? -1 : 1;
		}
	}
}

void FlyingBehavior::updateAnimation(const sf::Time& frameTime) {
	// calculate new game state and set animation.

	GameObjectState newState = GameObjectState::Idle;
	if (m_enemy->isDead()) {
		newState = GameObjectState::Dead;
	}
	else if (m_fightAnimationTime > sf::Time::Zero) {
		newState = m_fightAnimationState;
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

