#include "MovableGhost.h"
#include "Level/LevelMovableGameObject.h"
#include "SpellManager.h"
#include "Level/Level.h"

MovableGhost::MovableGhost(const AIWalkingQueryRecord& rec, Level* level, Screen* screen) : MovableGameObject() {
	m_level = level;
	m_aiRec = rec;

	m_avoidableTiles.insert(LevelDynamicTileID::SpikesTop);
	m_avoidableTiles.insert(LevelDynamicTileID::SpikesBottom);

	m_boundingBox = m_aiRec.boundingBox;
	setPosition(sf::Vector2f(m_boundingBox.left, m_boundingBox.top));

	m_debugger = new MovingGhostDebugger();
	screen->addObject(m_debugger);
}

MovableGhost::~MovableGhost() {
}

void MovableGhost::update(const sf::Time& frameTime) {
	if (m_record.collides) return;
	setAcceleration(sf::Vector2f(m_aiRec.accelerationX, m_aiRec.accelerationGravity));
	sf::Vector2f nextPosition;
	calculateNextPosition(frameTime, nextPosition);
	checkCollisions(nextPosition);

	MovableGameObject::update(frameTime);
	m_debugger->addDebugBoundingBox(*getBoundingBox());
	setAccelerationX(0.f);
}

void MovableGhost::checkCollisions(const sf::Vector2f& nextPosition) {

	const sf::FloatRect& bb = *getBoundingBox();

	sf::FloatRect nextBoundingBoxX(nextPosition.x, bb.top, bb.width, bb.height);
	sf::FloatRect nextBoundingBoxY(bb.left, nextPosition.y, bb.width, bb.height);
	sf::FloatRect nextBoundingBox(nextPosition.x, nextPosition.y, bb.width, bb.height);

	if (m_level->collidesWithDynamicTiles(nextBoundingBox, m_avoidableTiles)) {
		m_record.collides = true;
		m_record.evilTile = true;
		return;
	}

	WorldCollisionQueryRecord rec;
	rec.ignoreDynamicTiles = m_aiRec.ignoreDynamicTiles;

	bool isMovingDown = nextPosition.y > bb.top;
	bool isMovingRight = nextPosition.x > bb.left;

	// should we use strategy 2: try y direction first, then x direction?
	bool tryYfirst = false;
	rec.boundingBox = nextBoundingBoxX;
	rec.collisionDirection = isMovingRight ? CollisionDirection::Right : CollisionDirection::Left;
	if (m_level->collides(rec)) {
		if (std::abs(nextPosition.x - rec.safeLeft) > std::abs(getVelocity().x) + 10.f) {
			tryYfirst = true;
		}
	}

	if (!tryYfirst) {
		// check for collision on x axis
		rec.boundingBox = nextBoundingBoxX;
		rec.collisionDirection = isMovingRight ? CollisionDirection::Right : CollisionDirection::Left;

		if (m_level->collides(rec)) {
			setAccelerationX(0.f);
			setVelocityX(0.f);
			setPositionX(rec.safeLeft);
			nextBoundingBoxY.left = rec.safeLeft;
		}
		else {
			nextBoundingBoxY.left = nextPosition.x;
		}

		// check for collision on y axis
		rec.boundingBox = nextBoundingBoxY;
		rec.collisionDirection = isMovingDown ? CollisionDirection::Down : CollisionDirection::Up;

		if (m_level->collides(rec)) {
			m_record.collides = true;
			m_record.direction = rec.collisionDirection;
			m_record.savePosY = rec.safeTop;
			return;
		}
	}
	else {
		// check for collision on y axis
		rec.boundingBox = nextBoundingBoxY;
		rec.collisionDirection = isMovingDown ? CollisionDirection::Down : CollisionDirection::Up;

		if (m_level->collides(rec)) {
			m_record.collides = true;
			m_record.direction = rec.collisionDirection;
			m_record.savePosY = rec.safeTop;
			return;
		}
		
		nextBoundingBoxX.top = nextPosition.y;

		// check for collision on x axis
		rec.boundingBox = nextBoundingBoxX;
		rec.collisionDirection = isMovingRight ? CollisionDirection::Right : CollisionDirection::Left;

		if (m_level->collides(rec)) {
			setAccelerationX(0.f);
			setVelocityX(0.f);
			setPositionX(rec.safeLeft);
		}
	}

	if (!isMovingDown && nextBoundingBoxY.top < -bb.height ||
		isMovingDown && nextBoundingBoxY.top > m_level->getWorldRect().top + m_level->getWorldRect().height) {
		m_record.collides = true;
		m_record.evilTile = true;
		return;
	}
}

void MovableGhost::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	// distinguish damping in the air and at the ground
	float dampingPerSec = (getVelocity().y == 0.f) ? m_aiRec.dampingGroundPerS : m_aiRec.dampingAirPerS;
	// don't damp when there is active acceleration 
	if (getAcceleration().x != 0.f) dampingPerSec = 0.f;
	nextVel.x = (getVelocity().x + getAcceleration().x * frameTime.asSeconds()) * pow(1 - dampingPerSec, frameTime.asSeconds());
	nextVel.y = getVelocity().y + getAcceleration().y * frameTime.asSeconds();
}

float MovableGhost::getConfiguredMaxVelocityX() const {
	return m_aiRec.maxVelX;
}

float MovableGhost::getConfiguredMaxVelocityYDown() const {
	return m_aiRec.maxVelYDown;
}

float MovableGhost::getConfiguredMaxVelocityYUp() const {
	return m_aiRec.maxVelYUp;
}

const GhostRecord& MovableGhost::getGhostRecord() const {
	return m_record;
}

GameObjectType MovableGhost::getConfiguredType() const {
	return GameObjectType::_Undefined;
}

