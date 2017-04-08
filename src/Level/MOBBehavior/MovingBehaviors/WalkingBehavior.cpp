#include "Level/MOBBehavior/MovingBehaviors/WalkingBehavior.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "Level/MOBBehavior/JumpingGhost.h"

WalkingBehavior::WalkingBehavior(Enemy* enemy) :
	MovingBehavior(enemy),
	EnemyMovingBehavior(enemy) {
}

bool WalkingBehavior::doAIJump(bool onlyJump) {
	if (!m_enemy->isReady() || m_movingDirectionX == 0) {
		return false;
	}
	sf::FloatRect bb = *m_enemy->getBoundingBox();

	if (!onlyJump) {
		bb.left = m_isFacingRight ? bb.left + m_aiRecord.distanceToAbyss : bb.left - m_aiRecord.distanceToAbyss;
		bb.top = m_isFlippedGravity ? bb.top - 1.f : bb.top + 1.f;

		WorldCollisionQueryRecord rec;
		rec.boundingBox = bb;
		rec.collisionDirection = m_isFlippedGravity ? CollisionDirection::Up : CollisionDirection::Down;
		rec.ignoreDynamicTiles = m_ignoreDynamicTiles;
		if (m_enemy->getLevel()->collides(rec)) return false;
	} 

	// we did not collide with our ghost rec. check if we can jump or walk on.
	m_aiRecord.shouldJump = false;
	m_aiRecord.shouldWalk = false;
	
	m_aiRecord.isFlippedGravity = m_isFlippedGravity;
	m_aiRecord.accelerationGravity = m_isFlippedGravity ? -m_configuredGravity : m_configuredGravity;
	m_aiRecord.accelerationX = m_isFacingRight ? m_walkAcceleration : -m_walkAcceleration;
	m_aiRecord.maxVelX = m_maxVelocityX;
	m_aiRecord.maxVelYDown = m_maxVelocityYDown;
	m_aiRecord.maxVelYUp = m_maxVelocityYUp;
	m_aiRecord.dampingAirPerS = m_dampingAirPerS;
	m_aiRecord.dampingGroundPerS = m_dampingGroundPerS;
	m_aiRecord.isDropAlways = m_dropAlways;

	float landingYPosJump = -1.f;
	float landingYPosWalk = -1.f;
 
	m_aiRecord.boundingBox = *m_enemy->getBoundingBox();
	JumpingGhost* ghost = new JumpingGhost(m_aiRecord, m_mob->getLevel(), m_mob->getScreen());
	ghost->setVelocityY(m_isFlippedGravity ? m_configuredMaxVelocityYUp : -m_configuredMaxVelocityYUp);
	landingYPosJump = ghost->calculateJump();
	delete ghost;

	if (!onlyJump) {
		m_aiRecord.boundingBox = bb;
		ghost = new JumpingGhost(m_aiRecord, m_mob->getLevel(), m_mob->getScreen());
		landingYPosWalk = ghost->calculateJump();
		delete ghost;
	}

	if (landingYPosJump < landingYPosWalk) {
		// we can do a jump.
		m_aiRecord.shouldJump = true;
	}
	else if (landingYPosWalk > 0.f) {
		m_aiRecord.shouldWalk = true;
	}

	return true;
}

void WalkingBehavior::checkCollisions(const sf::Vector2f& nextPosition) {
	float oldPositionX = m_enemy->getPosition().x;

	if (m_walksBlindly && !m_isGrounded) {
		m_walksBlindly = false;
	}

	if (m_jumpsBlindly && m_isGrounded) {
		m_jumpsBlindly = false;
	}
	
	bool collidesY;
	MovingBehavior::checkXYDirection(nextPosition, m_collidesX, collidesY);
	sf::FloatRect nextBoundingBox(nextPosition.x, nextPosition.y, m_mob->getBoundingBox()->width, m_mob->getBoundingBox()->height);
	if (m_mob->getLevel()->collidesWithEvilTiles(nextBoundingBox)) {
		m_mob->setAccelerationX(0.f);
		m_mob->setVelocityX(0.f);
	}

	m_jumps = false;
	if (m_collidesX && m_isGrounded) {
		// would a jump work? 
		doAIJump(true);
		m_jumps = m_aiRecord.shouldJump;
	}
	else if (!m_walksBlindly && m_isGrounded && doAIJump(false)) {
		if (!m_aiRecord.shouldJump && !m_aiRecord.shouldWalk) {
			// we'd fall here. stop and change the direction we walk.
			m_enemy->setAccelerationX(0.f);
			m_enemy->setVelocityX(0.f);
			m_enemy->setPositionX(oldPositionX);
			m_collidesX = true; // it kind of collides. this is used for the enemy if it shall wait.
		}
		else if (m_aiRecord.shouldJump) {
			m_jumps = true;
		}
		else {
			// only walk
			m_walksBlindly = true;
			m_movingDirectionX = m_isFacingRight ? 1 : -1;
		}
	}

	// if the enemy collidesX but can't jump, it turns around and waits for a certain time.
	if (m_collidesX && m_isGrounded && !m_jumps) {
		m_enemy->setWaiting();
		m_movingDirectionX = m_isFacingRight ? -1 : 1;
	}
	if (m_jumps && m_movingDirectionX == 0) {
		m_movingDirectionX = m_isFacingRight ? 1 : -1;
		m_jumpsBlindly = true;
	}
}

void WalkingBehavior::handleTrueAcceleration() {
	if (m_jumps && m_isGrounded) {
		m_enemy->setVelocityY(-m_enemy->getConfiguredMaxVelocityYUp()); // first jump vel will always be max y vel. 
		m_jumps = false;
	}

	float newAccelerationY = m_isFlippedGravity ? -m_gravity : m_gravity;
	float newAccelerationX = m_enemy->getAcceleration().x + m_movingDirectionX * m_walkAcceleration;

	m_nextIsFacingRight = (m_movingDirectionX == 0) ? m_nextIsFacingRight : (m_movingDirectionX == 1);

	m_enemy->setAcceleration(sf::Vector2f(newAccelerationX, newAccelerationY));
}

void WalkingBehavior::calculateJumpHeight() {
	m_aiRecord.jumpHeight = m_configuredMaxVelocityYUp * m_configuredMaxVelocityYUp / (2 * m_configuredGravity);
}

void WalkingBehavior::setDistanceToAbyss(float distance) {
	m_aiRecord.distanceToAbyss = distance;
}

void WalkingBehavior::setDropAlways(bool dropAlways) {
	m_dropAlways = dropAlways;
}

void WalkingBehavior::setIgnoreDynamicTiles(bool value) {
	MovingBehavior::setIgnoreDynamicTiles(value);
	m_aiRecord.ignoreDynamicTiles = value;
}

float WalkingBehavior::getDistanceToAbyss() const {
	return m_aiRecord.distanceToAbyss;
}

void WalkingBehavior::makeRandomDecision() {
	if (!m_isGrounded || m_walksBlindly) return;
	m_movingDirectionX = rand() % 3 - 1;
}

void WalkingBehavior::updateAnimation(const sf::Time& frameTime) {
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
	else if (std::abs(m_enemy->getVelocity().x) > 20.f) {
		newState = GameObjectState::Walking;
	}

	// only update animation if we need to
	if (m_enemy->getState() != newState || (m_nextIsFacingRight != m_isFacingRight)) {
		m_isFacingRight = m_nextIsFacingRight;
		m_enemy->setState(newState);
		m_enemy->setCurrentAnimation(m_enemy->getAnimation(m_enemy->getState()), !m_isFacingRight);
	}
}
