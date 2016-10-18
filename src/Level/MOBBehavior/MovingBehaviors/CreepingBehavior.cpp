#include "Level/MOBBehavior/MovingBehaviors/CreepingBehavior.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "Level/DynamicTiles/MovingTile.h"

CreepingBehavior::CreepingBehavior(Enemy* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy) {
	m_creepingDirection = CreepingDirection::Bottom;

	// init sprite offset for animation
	auto const& frameSize = enemy->getAnimatedSprite().getGlobalBounds();
	auto const& bb = *enemy->getBoundingBox();
	m_horizontalSpriteOffset = enemy->getSpriteOffset();

	float diffWidth = frameSize.width - bb.width;
	float diffHeight = frameSize.height - bb.height;
	m_verticalSpriteOffset = sf::Vector2f(
		diffWidth - diffHeight,
		diffHeight - diffWidth) + m_horizontalSpriteOffset;
}

void CreepingBehavior::handleDefaultAcceleration() {
	float newAccelerationX = m_mob->getAcceleration().x;
	float newAccelerationY = 0.f;

	switch (m_creepingDirection) {
	case CreepingDirection::Bottom:
	default:
		newAccelerationY += getGravity();
		break;
	case CreepingDirection::Top:
		newAccelerationY -= getGravity();
		break;
	case CreepingDirection::Right:
		newAccelerationX += getGravity();
		break;
	case CreepingDirection::Left:
		newAccelerationX -= getGravity();
		break;
	}

	m_mob->setAcceleration(sf::Vector2f(newAccelerationX, newAccelerationY));
}

void CreepingBehavior::handleTrueAcceleration() {
	float newAccelerationX = 0.f;
	float newAccelerationY = 0.f;

	if (isCreepingOnX()) {
		newAccelerationY = m_creepingDirection == CreepingDirection::Top ? -m_gravity : m_gravity;
		newAccelerationX = m_enemy->getAcceleration().x + m_movingDirectionX * m_walkAcceleration;
		m_nextIsFacingRight = (m_movingDirectionX == 0) ? m_nextIsFacingRight : (m_movingDirectionX == 1);
	}
	else {
		newAccelerationX = m_enemy->getAcceleration().x + (m_creepingDirection == CreepingDirection::Left ? -m_gravity : m_gravity);
		newAccelerationY = m_movingDirectionY * m_walkAcceleration;
		m_nextIsFacingRight = (m_movingDirectionY == 0) ? m_nextIsFacingRight : (m_movingDirectionY == 1);
	}

	m_enemy->setAcceleration(sf::Vector2f(newAccelerationX, newAccelerationY));
}

void CreepingBehavior::checkCollisions(const sf::Vector2f& nextPosition) {
	bool collidesX;
	bool collidesY;
	checkCreepingXYDirection(nextPosition, collidesX, collidesY);
	sf::FloatRect nextBoundingBox(nextPosition.x, nextPosition.y, m_mob->getBoundingBox()->width, m_mob->getBoundingBox()->height);
	if (m_mob->getLevel()->collidesWithEvilTiles(nextBoundingBox)) {
		m_mob->setAcceleration(sf::Vector2f(0.f, 0.f));
		m_mob->setVelocity(sf::Vector2f(0.f, 0.f));
		return;
	}

	// check if we should creep up a wall
	if (collidesX && isCreepingOnX()) {
		m_movingDirectionY = m_creepingDirection == CreepingDirection::Bottom ? -1 : 1;
		bool isCreepingRight = nextPosition.x > m_mob->getPosition().x;
		auto const& bb = *m_mob->getBoundingBox();
		sf::Vector2f newPosition;
		if (m_creepingDirection == CreepingDirection::Bottom) {
			newPosition = m_mob->getPosition() + sf::Vector2f(
				isCreepingRight ? (bb.width - bb.height) : 0.f,
				bb.height - bb.width);
		}
		else {
			newPosition = m_mob->getPosition() + sf::Vector2f(
				isCreepingRight ? (bb.width - bb.height) : 0.f,
				0.f);
		}
		m_mob->setPosition(newPosition);
		setCreepingDirection(isCreepingRight ? CreepingDirection::Right : CreepingDirection::Left);
		return;
	}
	if (collidesY && !isCreepingOnX()) {
		m_movingDirectionX = m_creepingDirection == CreepingDirection::Right ? -1 : 1;
		bool isCreepingDown = nextPosition.y > m_mob->getPosition().y;
		auto const& bb = *m_mob->getBoundingBox();
		sf::Vector2f newPosition;
		if (m_creepingDirection == CreepingDirection::Right) {
			newPosition = m_mob->getPosition() + sf::Vector2f(
				bb.width - bb.height,
				isCreepingDown ? (bb.height - bb.width) : 0.f);
		}
		else {
			newPosition = m_mob->getPosition() + sf::Vector2f(
				0.f,
				isCreepingDown ? (bb.height - bb.width) : 0.f);
		}
		m_mob->setPosition(newPosition);
		setCreepingDirection(isCreepingDown ? CreepingDirection::Bottom : CreepingDirection::Top);
		return;
	}

	// check if we should creep down or up a wall
	if (isCreepingOnX() && m_movingDirectionX != 0 && m_isGrounded) {
		bool isMovingRight = m_movingDirectionX > 0;
		std::swap(nextBoundingBox.width, nextBoundingBox.height);
		nextBoundingBox.left = isMovingRight ?
			nextBoundingBox.left + 0.5f * nextBoundingBox.height :
			nextBoundingBox.left + 0.5f * (nextBoundingBox.height - nextBoundingBox.width);

		nextBoundingBox.top = m_creepingDirection == CreepingDirection::Bottom ?
			nextBoundingBox.top - 0.5f * (nextBoundingBox.height - nextBoundingBox.width) :
			nextBoundingBox.top - 0.5f * nextBoundingBox.height;

		WorldCollisionQueryRecord rec;
		rec.ignoreDynamicTiles = m_ignoreDynamicTiles;
		rec.boundingBox = nextBoundingBox;
		if (!m_mob->getLevel()->collides(rec)) {
			m_mob->setPosition(sf::Vector2f(nextBoundingBox.left, nextBoundingBox.top));
			m_movingDirectionY = m_creepingDirection == CreepingDirection::Bottom ? 1 : -1;
			setCreepingDirection(isMovingRight ? CreepingDirection::Left : CreepingDirection::Right);
		}
		return;
	}

	// check if we should creep again on the x axis
	if (!isCreepingOnX() && m_movingDirectionY != 0 && m_isGrounded) {
		bool isMovingDown = m_movingDirectionY > 0;
		nextBoundingBox.top = isMovingDown ?
			nextBoundingBox.top + 0.5f * nextBoundingBox.height :
			nextBoundingBox.top - 0.5f * nextBoundingBox.height;

		nextBoundingBox.left = m_creepingDirection == CreepingDirection::Right ?
			nextBoundingBox.left + 0.5f * nextBoundingBox.width :
			nextBoundingBox.left - 0.5f * nextBoundingBox.width;

		WorldCollisionQueryRecord rec;
		rec.ignoreDynamicTiles = m_ignoreDynamicTiles;
		rec.boundingBox = nextBoundingBox;
		if (!m_mob->getLevel()->collides(rec)) {
			m_mob->setPosition(sf::Vector2f(nextBoundingBox.left, nextBoundingBox.top));
			m_movingDirectionX = m_creepingDirection == CreepingDirection::Right ? 1 : -1;
			setCreepingDirection(isMovingDown ? CreepingDirection::Top : CreepingDirection::Bottom);
		}
		return;
	}
}

void CreepingBehavior::checkCreepingXYDirection(const sf::Vector2f& nextPosition, bool& collidesX, bool& collidesY) {
	collidesX = false;
	collidesY = false;

	const sf::FloatRect& bb = *m_mob->getBoundingBox();
	const Level& level = *m_mob->getLevel();
	sf::FloatRect nextBoundingBoxX(nextPosition.x, bb.top, bb.width, bb.height);
	sf::FloatRect nextBoundingBoxY(bb.left, nextPosition.y, bb.width, bb.height);

	WorldCollisionQueryRecord rec;
	rec.ignoreDynamicTiles = m_ignoreDynamicTiles;

	bool isMovingDown = nextPosition.y > bb.top;
	bool isMovingRight = nextPosition.x > bb.left;
	bool isFalling = isFallingDown(isMovingDown, isMovingRight);

	// should we use strategy 2: try y direction first, then x direction?
	bool tryYfirst = false;
	rec.boundingBox = nextBoundingBoxX;
	rec.collisionDirection = isMovingRight ? CollisionDirection::Right : CollisionDirection::Left;
	if (level.collides(rec)) {
		if (std::abs(nextPosition.x - rec.safeLeft) > std::abs(m_mob->getVelocity().x) + 10.f) {
			tryYfirst = true;
		}
	}

	if (!tryYfirst) {
		// check for collision on x axis
		rec.boundingBox = nextBoundingBoxX;
		rec.collisionDirection = isMovingRight ? CollisionDirection::Right : CollisionDirection::Left;
		collidesX = level.collides(rec);
		if (collidesX) {
			if (isFalling) {
				m_isGrounded = true;
			}
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
		// reset moving parent
		rec.movingParent = nullptr;
		collidesY = level.collides(rec);
		if (collidesY) {
			if (isFalling) {
				m_mob->setMovingParent(rec.movingParent);
				m_isGrounded = true;
			}
			m_mob->setAccelerationY(0.f);
			m_mob->setVelocityY(0.f);
			m_mob->setPositionY(rec.safeTop);
		}
	}
	else {
		// check for collision on y axis
		rec.boundingBox = nextBoundingBoxY;
		rec.collisionDirection = isMovingDown ? CollisionDirection::Down : CollisionDirection::Up;

		collidesY = level.collides(rec);
		if (collidesY) {
			if (isFalling) {
				m_isGrounded = true;
				m_mob->setMovingParent(rec.movingParent);
			}
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
			if (isFalling) {
				m_isGrounded = true;
			}
			m_mob->setAccelerationX(0.f);
			m_mob->setVelocityX(0.f);
			m_mob->setPositionX(rec.safeLeft);
		}
	}

	if (isCreepingOnX() && std::abs(m_mob->getVelocity().y) > 0.f || !isCreepingOnX() && std::abs(m_mob->getVelocity().x) > 0.f)
		m_isGrounded = false;

	if ((!isMovingDown && nextBoundingBoxY.top < -bb.height) ||
		(isMovingDown && nextBoundingBoxY.top > level.getWorldRect().top + level.getWorldRect().height)) {
		m_mob->setDead();
	}

	// check for wrong parent
	if (MovingTile* mt = m_mob->getMovingParent()) {
		if (m_creepingDirection == CreepingDirection::Bottom) {
			if (mt->getBoundingBox()->top + Epsilon < m_mob->getBoundingBox()->top + m_mob->getBoundingBox()->height) {
				m_mob->setMovingParent(nullptr);
			}
		}
		else {
			if (mt->getBoundingBox()->top + mt->getBoundingBox()->height > Epsilon + m_mob->getBoundingBox()->top) {
				m_mob->setMovingParent(nullptr);
			}
		}
	}
}

void CreepingBehavior::makeRandomDecision() {
	if (!m_isGrounded) return;
	m_movingDirectionX = rand() % 3 - 1;
	m_movingDirectionY = rand() % 3 - 1;
}

void CreepingBehavior::updateAnimation(const sf::Time& frameTime) {
	// calculate new game state and set animation.

	GameObjectState newState = GameObjectState::Idle;
	if (m_enemy->isDead()) {
		newState = GameObjectState::Dead;
	}
	else if (m_fightAnimationTime > sf::Time::Zero) {
		newState = m_fightAnimationState;
	}
	else if (!m_isGrounded) {
		newState = GameObjectState::Jumping;
	}
	else if (isCreepingOnX() && std::abs(m_enemy->getVelocity().x) > 20.f ||
		!isCreepingOnX() && std::abs(m_enemy->getVelocity().y) > 20.f) {
		newState = GameObjectState::Walking;
	}

	// only update animation if we need to
	if (m_enemy->getState() != newState || (m_nextIsFacingRight != m_isFacingRight)) {
		m_isFacingRight = m_nextIsFacingRight;
		m_enemy->setState(newState);
		m_enemy->setCurrentAnimation(m_enemy->getAnimation(m_enemy->getState()), m_creepingDirection != CreepingDirection::Right ? !m_isFacingRight : m_isFacingRight);
	}
}

bool CreepingBehavior::isCreepingOnX() const {
	return m_creepingDirection == CreepingDirection::Bottom || m_creepingDirection == CreepingDirection::Top;
}

bool CreepingBehavior::isFallingDown(bool isMovingDown, bool isMovingRight) const {
	return isMovingDown && m_creepingDirection == CreepingDirection::Bottom ||
		!isMovingDown && m_creepingDirection == CreepingDirection::Top ||
		isMovingRight && m_creepingDirection == CreepingDirection::Right ||
		!isMovingRight && m_creepingDirection == CreepingDirection::Left;
}

void CreepingBehavior::setCreepingDirection(CreepingDirection dir) {
	bool previouslyHorizontal = isCreepingOnX();
	m_creepingDirection = dir;
	bool creepingHorizontal = isCreepingOnX();
	if (creepingHorizontal == previouslyHorizontal) return;

	// swap velocities
	m_maxVelocityX = creepingHorizontal ? m_configuredMaxVelocityX : m_configuredMaxVelocityYDown;
	m_maxVelocityYDown = creepingHorizontal ? m_configuredMaxVelocityYDown : m_configuredMaxVelocityX;
	m_maxVelocityYUp = creepingHorizontal ? m_configuredMaxVelocityYUp : m_configuredMaxVelocityX;

	// swap bounding box
	auto newBB = *m_mob->getBoundingBox();
	std::swap(newBB.width, newBB.height);
	m_mob->setBoundingBox(newBB);

	// adjust animation
	m_enemy->setSpriteOffset(creepingHorizontal ? m_horizontalSpriteOffset : 
		m_verticalSpriteOffset);

	switch (m_creepingDirection) {
	case CreepingDirection::Bottom:
	default:
		m_enemy->getAnimatedSprite().setRotation(0.f);
		m_enemy->getAnimatedSprite().setFlippedY(false);
		break;
	case CreepingDirection::Top:
		m_enemy->getAnimatedSprite().setRotation(0.f);
		m_enemy->getAnimatedSprite().setFlippedY(true);
		break;
	case CreepingDirection::Left:
		m_enemy->getAnimatedSprite().setRotation(90.f);
		m_enemy->getAnimatedSprite().setFlippedX(false);
		m_enemy->getAnimatedSprite().setFlippedY(false);
		break;
	case CreepingDirection::Right:
		m_enemy->getAnimatedSprite().setRotation(270.f);
		m_enemy->getAnimatedSprite().setFlippedX(false);
		m_enemy->getAnimatedSprite().setFlippedY(false);
		break;
	}

	m_isCollisionTiltSuppressed = true;
}