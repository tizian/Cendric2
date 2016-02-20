#include "Level/MOBBehavior/MovingBehaviors/WalkingBehavior.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "MovableGhost.h"

WalkingBehavior::WalkingBehavior(Enemy* enemy) : 
	EnemyMovingBehavior(enemy),
	MovingBehavior(enemy) {
}

bool WalkingBehavior::doAIJump() {
	sf::FloatRect bb = *m_enemy->getBoundingBox();

	bb.left = m_isFacingRight ? bb.left + m_aiRecord.distanceToAbyss : bb.left - m_aiRecord.distanceToAbyss;
	bb.top = m_isFlippedGravity ? bb.top - 1.f : bb.top + 1.f;

	WorldCollisionQueryRecord rec;
	rec.boundingBox = bb;
	rec.ignoreDynamicTiles = m_ignoreDynamicTiles;
	if (m_enemy->getLevel()->collides(rec)) return false;

	// we did not collide with our ghost rec. check if we can jump or walk on.
	m_aiRecord.shouldJump = false;
	m_aiRecord.shouldWalk = false;
	
	m_aiRecord.accelerationGravity = m_isFlippedGravity ? -m_configuredGravity : m_configuredGravity;
	m_aiRecord.accelerationX = m_isFacingRight ? m_walkAcceleration : -m_walkAcceleration;
	m_aiRecord.maxVelX = m_maxVelocityX;
	m_aiRecord.maxVelYDown = m_maxVelocityYDown;
	m_aiRecord.maxVelYUp = m_maxVelocityYUp;
	m_aiRecord.dampingAirPerS = m_dampingAirPerS;
	m_aiRecord.dampingGroundPerS = m_dampingGroundPerS;

	const sf::Time fixedTimestep = sf::seconds(0.1f);

	float landingYPosJump = -1000.f;
	float landingYPosWalk = -1000.f;

	// check the jump for 2 sec. 
	m_aiRecord.boundingBox = *m_enemy->getBoundingBox();
	m_aiRecord.boundingBox.left = m_isFacingRight ? m_aiRecord.boundingBox.left - 1.f : m_aiRecord.boundingBox.left + 1.f;
	MovableGhost* ghost = new MovableGhost(m_aiRecord, m_mob->getLevel(), m_mob->getScreen());
	ghost->setVelocityY(m_isFlippedGravity ? m_configuredMaxVelocityYUp : -m_configuredMaxVelocityYUp);
	for (int i = 1; i < 20; ++i) {
		ghost->update(fixedTimestep);
		if (ghost->getGhostRecord().collides) {

			// got a collision! is it a good collision?
			if (!ghost->getGhostRecord().evilTile && (
				ghost->getGhostRecord().direction == CollisionDirection::Down && !m_isFlippedGravity || 
				ghost->getGhostRecord().direction == CollisionDirection::Up && m_isFlippedGravity)) {
				landingYPosJump = ghost->getGhostRecord().savePosY;
				// is this position feasible?
				if (std::abs(landingYPosJump - m_enemy->getPosition().y) > m_jumpHeight - 5.f) {
					// not feasible, sorry.
					landingYPosJump = -1000.f;
				}
			}
			// bad collision. 
			break;
		}
	}

	delete ghost;
	m_aiRecord.boundingBox = bb;
	ghost = new MovableGhost(m_aiRecord, m_mob->getLevel(), m_mob->getScreen());

	for (int i = 1; i < 20; ++i) {
		ghost->update(fixedTimestep);
		if (ghost->getGhostRecord().collides) {

			// got a collision! is it a good collision?
			if (!ghost->getGhostRecord().evilTile && (
				ghost->getGhostRecord().direction == CollisionDirection::Down && !m_isFlippedGravity ||
				ghost->getGhostRecord().direction == CollisionDirection::Up && m_isFlippedGravity)) {
				landingYPosWalk = ghost->getGhostRecord().savePosY;
				// is this position feasible?
				if (std::abs(landingYPosWalk - m_enemy->getPosition().y) > m_jumpHeight - 5.f) {
					// not feasible, sorry.
					landingYPosWalk = -1000.f;
				}
			}
			// bad collision. 
			break;
		}
	}

	delete ghost;

	if (landingYPosJump > landingYPosWalk) {
		// we can do a jump.
		m_aiRecord.shouldJump = true;
		m_aiRecord.shouldWalk = true;
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
	if (m_collidesX) {
		// would a jump work? 
		m_jumps = !level.collidesAfterJump(bb, m_jumpHeight, m_isFacingRight, m_ignoreDynamicTiles);
	}
	else if (!m_walksBlindly && m_isGrounded && doAIJump()) {
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
		}
	}

	// if the enemy collidesX but can't jump, it turns around and wait for a certain time.
	if (m_collidesX && !m_jumps) {
		m_enemy->setWaiting();
		m_randomDecision = m_isFacingRight ? -1 : 1;
	}
}

void WalkingBehavior::calculateJumpHeight() {
	m_jumpHeight = m_configuredMaxVelocityYUp * m_configuredMaxVelocityYUp / (2 * m_configuredGravity);
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
	m_randomDecision = rand() % 3 - 1;
}

int WalkingBehavior::getRandomDecision() const {
	return m_randomDecision;
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
