#include "Level/EnemyBehavior/WalkingBehavior.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"

void WalkingBehavior::checkCollisions(const sf::Vector2f& nextPosition) {
	const sf::FloatRect& bb = *m_enemy->getBoundingBox();
	const Level& level = *m_enemy->getLevel();
	sf::FloatRect nextBoundingBoxX(nextPosition.x, bb.top, bb.width, bb.height);
	sf::FloatRect nextBoundingBoxY(bb.left, nextPosition.y, bb.width, bb.height);

	bool isMovingDown = nextPosition.y > bb.top; // the mob is always moving either up or down, because of gravity. There are very, very rare, nearly impossible cases where they just cancel out.
	bool isMovingX = nextPosition.x != bb.left;

	// check for collision on x axis
	bool collidesX = false;
	if (isMovingX && level.collides(nextBoundingBoxX, m_enemy->m_ignoreDynamicTiles)) {
		collidesX = true;
		m_enemy->setAccelerationX(0.0f);
		m_enemy->setVelocityX(0.0f);
	}
	else {
		nextBoundingBoxY.left = nextPosition.x;
	}
	// check for collision on y axis
	bool collidesY = level.collides(nextBoundingBoxY, m_enemy->m_ignoreDynamicTiles);

	if (!isMovingDown && collidesY) {
		m_enemy->setAccelerationY(0.0);
		m_enemy->setVelocityY(0.0f);
		// set mob up in case of anti gravity!
		if (m_enemy->getIsUpsideDown()) {
			m_enemy->setPositionY(m_enemy->m_level->getCeiling(nextBoundingBoxY));
			m_enemy->m_isGrounded = true;
		}
	}
	else if (isMovingDown && collidesY) {
		m_enemy->setAccelerationY(0.0f);
		m_enemy->setVelocityY(0.0f);
		// set mob down. in case of normal gravity.
		if (!m_enemy->getIsUpsideDown()) {
			m_enemy->setPositionY(level.getGround(nextBoundingBoxY));
			m_enemy->m_isGrounded = true;
		}
	}

	m_jumps = false;
	if (isMovingX && collidesX) {
		// would a jump work? 
		m_jumps = !m_enemy->m_level->collidesAfterJump(bb, m_jumpHeight, m_enemy->m_isFacingRight, m_enemy->m_ignoreDynamicTiles);
	}

	// checks if the enemy falls would fall deeper than it can jump. 
	if (!collidesX && isMovingX && m_enemy->m_level->fallsDeep(bb, m_jumpHeight, m_enemy->m_isFacingRight, getDistanceToAbyss(), m_enemy->m_ignoreDynamicTiles)) {
		m_enemy->setAccelerationX(0.0f);
		m_enemy->setVelocityX(0.0f);
		collidesX = true; // it kind of collides. this is used for the enemy if it shall wait.
	}

	if (std::abs(m_enemy->getVelocity().y) > 0.f)
		m_enemy->m_isGrounded = false;

	// if the enemy collidesX but can't jump and is chasing, it waits for a certain time.
	if (m_enemy->m_enemyState == EnemyState::Chasing && collidesX && !m_jumps) {
		m_enemy->m_waitingTime = m_enemy->getConfiguredWaitingTime();
	}
}

void WalkingBehavior::handleMovementInput() {
	// movement AI
	float newAccelerationX = m_enemy->getAcceleration().x;

	bool hasTarget = m_enemy->getCurrentTarget() != nullptr;
	sf::Vector2f center = m_enemy->getCenter();
	sf::Vector2f targetCenter = hasTarget ? m_enemy->getCurrentTarget()->getCenter() : center;
	if (hasTarget && (m_enemy->m_enemyState == EnemyState::Chasing || m_enemy->m_enemyState == EnemyState::Recovering)) {

		if (targetCenter.x < center.x && std::abs(targetCenter.x - center.x) > getApproachingDistance()) {
			m_enemy->m_nextIsFacingRight = false;
			newAccelerationX -= m_enemy->getConfiguredWalkAcceleration();
		}
		else if (targetCenter.x > center.x && std::abs(targetCenter.x - center.x) > getApproachingDistance()) {
			m_enemy->m_nextIsFacingRight = true;
			newAccelerationX += m_enemy->getConfiguredWalkAcceleration();
		}

		if (m_jumps && m_enemy->m_isGrounded) {
			m_enemy->setVelocityY(m_enemy->m_isFlippedGravity ? m_enemy->getConfiguredMaxVelocityYUp() : -m_enemy->getConfiguredMaxVelocityYUp()); // first jump vel will always be max y vel. 
			m_jumps = false;
		}
	}
	else if (hasTarget && m_enemy->m_enemyState == EnemyState::Fleeing) {

		if (targetCenter.x < center.x) {
			m_enemy->m_nextIsFacingRight = true;
			newAccelerationX += m_enemy->getConfiguredWalkAcceleration();
		}
		else if (targetCenter.x > center.x) {
			m_enemy->m_nextIsFacingRight = false;
			newAccelerationX -= m_enemy->getConfiguredWalkAcceleration();
		}

		if (m_jumps && m_enemy->m_isGrounded) {
			m_enemy->setVelocityY(-m_enemy->getConfiguredMaxVelocityYUp()); // first jump vel will always be max y vel. 
			m_jumps = false;
		}
	}
	else if (m_enemy->m_enemyState == EnemyState::Idle || m_enemy->m_enemyState == EnemyState::Waiting) {
		if (m_enemy->isAlly()) {
			sf::Vector2f mainCharCenter = m_enemy->m_mainChar->getCenter();

			if (mainCharCenter.x < center.x && std::abs(mainCharCenter.x - center.x) > 2 * getApproachingDistance()) {
				m_enemy->m_nextIsFacingRight = false;
				newAccelerationX -= m_enemy->getConfiguredWalkAcceleration();
			}
			else if (mainCharCenter.x > center.x && std::abs(mainCharCenter.x - center.x) > 2 * getApproachingDistance()) {
				m_enemy->m_nextIsFacingRight = true;
				newAccelerationX += m_enemy->getConfiguredWalkAcceleration();
			}

			if (m_jumps && m_enemy->m_isGrounded) {
				m_enemy->setVelocityY(m_enemy->m_isFlippedGravity ? m_enemy->getConfiguredMaxVelocityYUp() : -m_enemy->getConfiguredMaxVelocityYUp()); // first jump vel will always be max y vel. 
				m_jumps = false;
			}
		}
		else {
			if (m_randomDecision != 0) {
				m_enemy->m_nextIsFacingRight = m_randomDecision == 1;
				newAccelerationX += m_randomDecision * m_enemy->getConfiguredWalkAcceleration();
			}
		}
	}

	m_enemy->setAcceleration(sf::Vector2f(newAccelerationX, (m_enemy->m_isFlippedGravity ? -m_enemy->getGravityAcceleration() : m_enemy->getGravityAcceleration())));
}

void WalkingBehavior::setJumpHeight(float height) {
	m_jumpHeight = height;
}

float WalkingBehavior::getJumpHeight() const {
	return m_jumpHeight;
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
	else if (m_enemy->m_fightAnimationTime > sf::Time::Zero) {
		newState = GameObjectState::Fighting;
	}
	else if (!m_enemy->m_isGrounded) {
		newState = GameObjectState::Jumping;
	}
	else if (std::abs(m_enemy->getVelocity().x) > 20.0f) {
		newState = GameObjectState::Walking;
	}

	// only update animation if we need to
	if (m_enemy->m_state != newState || (m_enemy->m_nextIsFacingRight != m_enemy->m_isFacingRight)) {
		m_enemy->m_isFacingRight = m_enemy->m_nextIsFacingRight;
		m_enemy->m_state = newState;
		m_enemy->setCurrentAnimation(m_enemy->getAnimation(m_enemy->m_state), !m_enemy->m_isFacingRight);
	}
}
