#include "Level/WalkingEnemy.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"

WalkingEnemy::WalkingEnemy(Level* level, Screen* screen, bool isControlled) : 
	Enemy(level, screen, isControlled), 
	LevelMovableGameObject(level) {
}

WalkingEnemy::~WalkingEnemy() {
}

void WalkingEnemy::checkCollisions(const sf::Vector2f& nextPosition) {
	sf::FloatRect nextBoundingBoxX(nextPosition.x, getBoundingBox()->top, getBoundingBox()->width, getBoundingBox()->height);
	sf::FloatRect nextBoundingBoxY(getBoundingBox()->left, nextPosition.y, getBoundingBox()->width, getBoundingBox()->height);

	bool isMovingDown = nextPosition.y > getBoundingBox()->top; // the mob is always moving either up or down, because of gravity. There are very, very rare, nearly impossible cases where they just cancel out.
	bool isMovingX = nextPosition.x != getBoundingBox()->left;

	// check for collision on x axis
	bool collidesX = false;
	if (isMovingX && m_level->collides(nextBoundingBoxX, m_ignoreDynamicTiles)) {
		collidesX = true;
		setAccelerationX(0.0f);
		setVelocityX(0.0f);
	}
	else {
		nextBoundingBoxY.left = nextPosition.x;
	}
	// check for collision on y axis
	bool collidesY = m_level->collides(nextBoundingBoxY, m_ignoreDynamicTiles);

	if (!isMovingDown && collidesY) {
		setAccelerationY(0.0);
		setVelocityY(0.0f);
		// set mob up in case of anti gravity!
		if (getIsUpsideDown()) {
			setPositionY(m_level->getCeiling(nextBoundingBoxY));
			m_isGrounded = true;
		}
	}
	else if (isMovingDown && collidesY) {
		setAccelerationY(0.0f);
		setVelocityY(0.0f);
		// set mob down. in case of normal gravity.
		if (!getIsUpsideDown()) {
			setPositionY(m_level->getGround(nextBoundingBoxY));
			m_isGrounded = true;
		}
	}

	m_jumps = false;
	if (isMovingX && collidesX) {
		// would a jump work? 
		m_jumps = !m_level->collidesAfterJump(*getBoundingBox(), m_jumpHeight, m_isFacingRight, m_ignoreDynamicTiles);
	}

	// checks if the enemy falls would fall deeper than it can jump. 
	if (!collidesX && isMovingX && m_level->fallsDeep(*getBoundingBox(), m_jumpHeight, m_isFacingRight, getDistanceToAbyss(), m_ignoreDynamicTiles)) {
		setAccelerationX(0.0f);
		setVelocityX(0.0f);
		collidesX = true; // it kind of collides. this is used for the enemy if it shall wait.
	}

	if (std::abs(getVelocity().y) > 0.f)
		m_isGrounded = false;

	// if the enemy collidesX but can't jump and is chasing, it waits for a certain time.
	// the same if it can't reach the main char because of the y difference.
	if (m_enemyState == EnemyState::Chasing && ((collidesX && !m_jumps) || abs(m_mainChar->getPosition().y - getPosition().y) > 2 * m_jumpHeight)) {
		m_waitingTime = getConfiguredWaitingTime();
	}
}

void WalkingEnemy::makeRandomDecision() {
	m_randomDecision = rand() % 3 - 1;
}

void WalkingEnemy::handleMovementInput() {
	if (m_isDead) {
		setAcceleration(sf::Vector2f(0.f, getConfiguredGravityAcceleration()));
		return;
	}
	// movement AI
	float newAccelerationX = 0;
	
	bool hasTarget = m_currentTarget != nullptr;
	sf::Vector2f center = getCenter();
	sf::Vector2f targetCenter = hasTarget ? m_currentTarget->getCenter() : center;
	if (hasTarget && (m_enemyState == EnemyState::Chasing || m_enemyState == EnemyState::Recovering)) {

		if (targetCenter.x < center.x && std::abs(targetCenter.x - center.x) > getApproachingDistance()) {
			m_nextIsFacingRight = false;
			newAccelerationX -= getConfiguredWalkAcceleration();
		}
		else if (targetCenter.x > center.x && std::abs(targetCenter.x - center.x) > getApproachingDistance()) {
			m_nextIsFacingRight = true;
			newAccelerationX += getConfiguredWalkAcceleration();
		}

		if (m_jumps && m_isGrounded) {
			setVelocityY(m_isFlippedGravity ? getConfiguredMaxVelocityYUp() : -getConfiguredMaxVelocityYUp()); // first jump vel will always be max y vel. 
			m_jumps = false;
		}
	}
	else if (hasTarget && m_enemyState == EnemyState::Fleeing) {

		if (targetCenter.x < center.x) {
			m_nextIsFacingRight = true;
			newAccelerationX += getConfiguredWalkAcceleration();
		}
		else if (targetCenter.x > center.x) {
			m_nextIsFacingRight = false;
			newAccelerationX -= getConfiguredWalkAcceleration();
		}

		if (m_jumps && m_isGrounded) {
			setVelocityY(-getConfiguredMaxVelocityYUp()); // first jump vel will always be max y vel. 
			m_jumps = false;
		}
	}
	else if (m_enemyState == EnemyState::Idle || m_enemyState == EnemyState::Waiting) {
		if (m_isControlled) {
			sf::Vector2f mainCharCenter = m_mainChar->getCenter();

			if (mainCharCenter.x < center.x && std::abs(mainCharCenter.x - center.x) > 2 * getApproachingDistance()) {
				m_nextIsFacingRight = false;
				newAccelerationX -= getConfiguredWalkAcceleration();
			}
			else if (mainCharCenter.x > center.x && std::abs(mainCharCenter.x - center.x) > 2 * getApproachingDistance()) {
				m_nextIsFacingRight = true;
				newAccelerationX += getConfiguredWalkAcceleration();
			}

			if (m_jumps && m_isGrounded) {
				setVelocityY(m_isFlippedGravity ? getConfiguredMaxVelocityYUp() : -getConfiguredMaxVelocityYUp()); // first jump vel will always be max y vel. 
				m_jumps = false;
			}
		}
		else {
			if (m_randomDecision != 0) {
				m_nextIsFacingRight = m_randomDecision == 1;
				newAccelerationX += m_randomDecision * getConfiguredWalkAcceleration();
			}
		}
	}

	setAcceleration(sf::Vector2f(newAccelerationX, (m_isFlippedGravity ? -getGravityAcceleration() : getGravityAcceleration())));
}

float WalkingEnemy::getDistanceToAbyss() const {
	return 10.f;
}
