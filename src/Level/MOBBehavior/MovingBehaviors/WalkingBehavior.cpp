#include "Level/MOBBehavior/MovingBehaviors/WalkingBehavior.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"

WalkingBehavior::WalkingBehavior(Enemy* enemy) : 
	EnemyMovingBehavior(enemy),
	MovingBehavior(enemy) {
}

void WalkingBehavior::checkCollisions(const sf::Vector2f& nextPosition) {
	const sf::FloatRect& bb = *m_enemy->getBoundingBox();
	const Level& level = *m_enemy->getLevel();
	float oldPositionX = m_enemy->getPosition().x;
	sf::FloatRect nextBoundingBoxX(nextPosition.x, bb.top, bb.width, bb.height);
	sf::FloatRect nextBoundingBoxY(bb.left, nextPosition.y, bb.width, bb.height);
	WorldCollisionQueryRecord rec;
	rec.ignoreDynamicTiles = m_ignoreDynamicTiles;

	bool isMovingDown = nextPosition.y > bb.top;
	bool isMovingRight = nextPosition.x > bb.left;

	// check for collision on x axis
	rec.boundingBox = nextBoundingBoxX;
	rec.collisionDirection = isMovingRight ? CollisionDirection::Right : CollisionDirection::Left;
	bool collidesX = false;
	if (level.collides(rec)) {
		collidesX = true;
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
	bool isFalling = isUpsideDown() != isMovingDown;
	rec.checkMovingPlatforms = isFalling;
	rec.upsideDown = isUpsideDown();

	rec.gainedRelativeVelocity = sf::Vector2f(0.f, 0.f);
	bool collidesY = level.collides(rec);
	m_enemy->setRelativeVelocity(rec.gainedRelativeVelocity);

	if (collidesY) {
		if (isFalling) {
			m_isGrounded = true;
		}
		m_enemy->setAccelerationY(0.f);
		m_enemy->setVelocityY(0.f);
		m_enemy->setPositionY(rec.safeTop);
	}

	m_jumps = false;
	if (collidesX) {
		// would a jump work? 
		m_jumps = !level.collidesAfterJump(bb, m_jumpHeight, m_isFacingRight, m_ignoreDynamicTiles);
	}

	// checks if the enemy falls would fall deeper than it can jump. 
	if (!collidesX && level.fallsDeep(bb, m_jumpHeight, m_isFacingRight, getDistanceToAbyss(), m_ignoreDynamicTiles)) {
		m_enemy->setAccelerationX(0.f);
		m_enemy->setVelocityX(0.f);
		m_enemy->setPositionX(oldPositionX);
		collidesX = true; // it kind of collides. this is used for the enemy if it shall wait.
	}

	if (std::abs(m_enemy->getVelocity().y) > 0.f)
		m_isGrounded = false;

	// if the enemy collidesX but can't jump and is chasing, it waits for a certain time.
	if (m_enemy->getEnemyState() == EnemyState::Chasing && collidesX && !m_jumps) {
		m_enemy->setWaiting();
	}

	if (!isMovingDown && nextBoundingBoxY.top < -bb.height ||
		isMovingDown && nextBoundingBoxY.top > level.getWorldRect().top + level.getWorldRect().height) {
		m_mob->setDead();
	}
}

void WalkingBehavior::handleMovementInput() {
	if (m_enemy->isDead()) {
		m_enemy->setAcceleration(sf::Vector2f(0.f, getGravity()));
		return;
	}

	// movement AI
	float newAccelerationX = m_enemy->getAcceleration().x;

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

		if (m_jumps && m_isGrounded) {
			m_enemy->setVelocityY(m_isFlippedGravity ? m_configuredMaxVelocityYUp : -m_configuredMaxVelocityYUp); // first jump vel will always be max y vel. 
			m_jumps = false;
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

		if (m_jumps && m_isGrounded) {
			m_enemy->setVelocityY(-m_enemy->getConfiguredMaxVelocityYUp()); // first jump vel will always be max y vel. 
			m_jumps = false;
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

			if (m_jumps && m_isGrounded) {
				m_enemy->setVelocityY(m_isFlippedGravity ? m_configuredMaxVelocityYUp : -m_configuredMaxVelocityYUp); // first jump vel will always be max y vel. 
				m_jumps = false;
			}
		}
		else {
			if (m_randomDecision != 0) {
				m_nextIsFacingRight = m_randomDecision == 1;
				newAccelerationX += m_randomDecision * m_walkAcceleration;
			}
		}
	}

	m_enemy->setAcceleration(sf::Vector2f(newAccelerationX, (m_isFlippedGravity ? -m_gravity : m_gravity)));
}

void WalkingBehavior::calculateJumpHeight() {
	m_jumpHeight = m_configuredMaxVelocityYUp * m_configuredMaxVelocityYUp / (2 * m_configuredGravity);
}

void WalkingBehavior::setDistanceToAbyss(float distance) {
	m_distanceToAbyss = distance;
}

float WalkingBehavior::getDistanceToAbyss() const {
	return m_distanceToAbyss;
}

void WalkingBehavior::makeRandomDecision() {
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
	else if (std::abs(m_enemy->getVelocity().x) > 20.0f) {
		newState = GameObjectState::Walking;
	}

	// only update animation if we need to
	if (m_enemy->getState() != newState || (m_nextIsFacingRight != m_isFacingRight)) {
		m_isFacingRight = m_nextIsFacingRight;
		m_enemy->setState(newState);
		m_enemy->setCurrentAnimation(m_enemy->getAnimation(m_enemy->getState()), !m_isFacingRight);
	}
}
