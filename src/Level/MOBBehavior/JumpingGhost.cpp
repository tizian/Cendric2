#include "Level/MOBBehavior/JumpingGhost.h"
#include "Level/LevelMovableGameObject.h"
#include "Spells/SpellManager.h"
#include "Level/Level.h"

JumpingGhost::JumpingGhost(const AIWalkingQueryRecord& rec, const Level* level, Screen* screen) : MovableGameObject() {
	m_level = level;
	m_aiRec = rec;

	m_boundingBox = m_aiRec.boundingBox;
	setPosition(sf::Vector2f(m_boundingBox.left, m_boundingBox.top));
	
	if (g_resourceManager->getConfiguration().isDebugRendering) {
		m_debugger = new JumpingGhostDebugger();
		screen->addObject(m_debugger);
	}
}

JumpingGhost::~JumpingGhost() {
}

void JumpingGhost::update(const sf::Time& frameTime) {
	if (m_record.collides) return;
	setAcceleration(sf::Vector2f(m_aiRec.accelerationX, m_aiRec.accelerationGravity));
	sf::Vector2f nextPosition;
	calculateNextPosition(frameTime, nextPosition);
	checkCollisions(nextPosition);

	MovableGameObject::update(frameTime);
	if (m_debugger != nullptr) m_debugger->addDebugBoundingBox(*getBoundingBox());
	setAccelerationX(0.f);
}

float JumpingGhost::calculateJump() {
	const sf::Time fixedTimestep = sf::seconds(0.1f);
	float landingPosY = -1.f;

	for (int i = 1; i < 20; ++i) {
		update(fixedTimestep);
		if (m_record.collides) {

			// got a collision! is it a good collision?
			if (!m_record.evilTile && (
				(m_record.direction == CollisionDirection::Down && !m_aiRec.isFlippedGravity) ||
				(m_record.direction == CollisionDirection::Up && m_aiRec.isFlippedGravity))) {
				landingPosY = m_record.savePosY;
				// is this position feasible?
				if (std::abs(landingPosY - m_aiRec.boundingBox.top) > m_aiRec.jumpHeight - 5.f) {
					// not feasible, sorry.
					return -1.f;
				}
				// does this position differ enough from the one we started?
				if (dist(getPosition(), sf::Vector2f(m_aiRec.boundingBox.left, m_aiRec.boundingBox.top)) < TILE_SIZE_F * 0.5f) {
					return -1.f;
				}
				else if (m_debugger != nullptr) {
					m_debugger->setGoodTrajectory();
				}
			}
			// bad collision. 
			break;
		}
		if (std::abs(getPosition().y - m_aiRec.boundingBox.top) > m_aiRec.jumpHeight) {
			break;
		}
	}

	return landingPosY;
}

void JumpingGhost::checkCollisions(const sf::Vector2f& nextPosition) {

	const sf::FloatRect& bb = *getBoundingBox();

	sf::FloatRect nextBoundingBoxX(nextPosition.x, bb.top, bb.width, bb.height);
	sf::FloatRect nextBoundingBoxY(bb.left, nextPosition.y, bb.width, bb.height);
	sf::FloatRect nextBoundingBox(nextPosition.x, nextPosition.y, bb.width, bb.height);

	WorldCollisionQueryRecord rec;
	rec.ignoreDynamicTiles = m_aiRec.ignoreDynamicTiles;

	bool isMovingDown = nextPosition.y > bb.top;
	bool isMovingRight = nextPosition.x > bb.left;
	bool isFalling = m_aiRec.isFlippedGravity != isMovingDown;

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
			setAccelerationY(0.f);
			setVelocityY(0.f);
			setPositionY(rec.safeTop);
			
			if (isFalling) {
				m_record.collides = true;
				m_record.direction = rec.collisionDirection;
				m_record.savePosY = rec.safeTop;
			}
		}
	}
	else {
		// check for collision on y axis
		rec.boundingBox = nextBoundingBoxY;
		rec.collisionDirection = isMovingDown ? CollisionDirection::Down : CollisionDirection::Up;

		if (m_level->collides(rec)) {
			setAccelerationY(0.f);
			setVelocityY(0.f);
			setPositionY(rec.safeTop);
			
			if (isFalling) {
				m_record.collides = true;
				m_record.direction = rec.collisionDirection;
				m_record.savePosY = rec.safeTop;
			}
		}
		else {
			nextBoundingBoxX.top = nextPosition.y;
		}

		// check for collision on x axis
		rec.boundingBox = nextBoundingBoxX;
		rec.collisionDirection = isMovingRight ? CollisionDirection::Right : CollisionDirection::Left;

		if (m_level->collides(rec)) {
			setAccelerationX(0.f);
			setVelocityX(0.f);
			setPositionX(rec.safeLeft);
		}
	}

	if (m_record.collides) return;

	if ((!isMovingDown && nextBoundingBoxY.top < -bb.height) ||
		(isMovingDown && nextBoundingBoxY.top > m_level->getWorldRect().top + m_level->getWorldRect().height)) {
		m_record.collides = true;
		m_record.evilTile = true;
		return;
	}

	nextBoundingBox.left = nextBoundingBoxX.left;
	nextBoundingBox.top = nextBoundingBoxY.top;
	if (m_level->collidesWithAvoidableTiles(nextBoundingBox)) {
		m_record.collides = true;
		m_record.evilTile = true;
		return;
	}
}

void JumpingGhost::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	// distinguish damping in the air and at the ground
	float dampingPerSec = (getVelocity().y == 0.f) ? m_aiRec.dampingGroundPerS : m_aiRec.dampingAirPerS;
	// don't damp when there is active acceleration 
	if (getAcceleration().x != 0.f) dampingPerSec = 0.f;
	nextVel.x = (getVelocity().x + getAcceleration().x * frameTime.asSeconds()) * pow(1 - dampingPerSec, frameTime.asSeconds());
	nextVel.y = getVelocity().y + getAcceleration().y * frameTime.asSeconds();
}

float JumpingGhost::getConfiguredMaxVelocityX() const {
	return m_aiRec.maxVelX;
}

float JumpingGhost::getConfiguredMaxVelocityYDown() const {
	return m_aiRec.maxVelYDown;
}

float JumpingGhost::getConfiguredMaxVelocityYUp() const {
	return m_aiRec.maxVelYUp;
}

const GhostRecord& JumpingGhost::getGhostRecord() const {
	return m_record;
}

GameObjectType JumpingGhost::getConfiguredType() const {
	return GameObjectType::_Undefined;
}

