#include "Level/MOBBehavior/MovingBehaviors/WalkingBehavior.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "Level/MOBBehavior/JumpingGhost.h"

WalkingBehavior::WalkingBehavior(Enemy* enemy) :
	EnemyMovingBehavior(enemy),
	MovingBehavior(enemy) {
}

bool WalkingBehavior::doAIJump(bool onlyJump) {
	if (m_enemy->isDead()) {
		return false;
	}
	sf::FloatRect bb = *m_enemy->getBoundingBox();

	if (!onlyJump) {
		bb.left = m_isFacingRight ? bb.left + m_aiRecord.distanceToAbyss : bb.left - m_aiRecord.distanceToAbyss;
		bb.top = m_isFlippedGravity ? bb.top - 1.f : bb.top + 1.f;

		WorldCollisionQueryRecord rec;
		rec.boundingBox = bb;
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

	if (landingYPosJump > landingYPosWalk) {
		// we can do a jump.
		m_aiRecord.shouldJump = true;
	}
	else if (landingYPosWalk > 0.f) {
		m_aiRecord.shouldWalk = true;
	}

	return true;
}

void WalkingBehavior::checkCollisions(const sf::Vector2f& nextPosition) {
	const sf::FloatRect& bb = *m_enemy->getBoundingBox();
	const Level& level = *m_enemy->getLevel();
	float oldPositionX = m_enemy->getPosition().x;

	if (m_walksBlindly && !m_isGrounded) {
		m_walksBlindly = false;
	}
	
	bool collidesY;
	MovingBehavior::checkXYDirection(nextPosition, m_collidesX, collidesY);
	
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
			m_walkingDirection = m_isFacingRight ? 1 : -1;
		}
	}

	// if the enemy collidesX but can't jump, it turns around and waits for a certain time.
	if (m_collidesX && m_isGrounded && !m_jumps) {
		m_enemy->setWaiting();
		m_walkingDirection = m_isFacingRight ? -1 : 1;
	}
	if (m_jumps && m_walkingDirection == 0) {
		m_walkingDirection = m_isFacingRight ? 1 : -1;
	}
}

void WalkingBehavior::calculateJumpHeight() {
	m_aiRecord.jumpHeight = m_configuredMaxVelocityYUp * m_configuredMaxVelocityYUp / (2 * m_configuredGravity);
}

void WalkingBehavior::setDistanceToAbyss(float distance) {
	m_aiRecord.distanceToAbyss = distance;
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
	m_walkingDirection = rand() % 3 - 1;
}

int WalkingBehavior::getWalkingDirection() const {
	return m_walkingDirection;
}

void WalkingBehavior::updateAnimation() {
	// calculate new game state and set animation.

	GameObjectState newState = GameObjectState::Idle;
	if (m_enemy->isDead()) {
		newState = GameObjectState::Dead;
	}
	else if (m_fightAnimationTime > sf::Time::Zero) {
		newState = GameObjectState::Fighting;
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
