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

