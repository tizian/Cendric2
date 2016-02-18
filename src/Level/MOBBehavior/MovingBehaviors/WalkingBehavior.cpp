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

	bool collidesY;
	MovingBehavior::checkXYDirection(nextPosition, m_collidesX, collidesY);

	m_jumps = false;
	if (m_collidesX) {
		// would a jump work? 
		m_jumps = !level.collidesAfterJump(bb, m_jumpHeight, m_isFacingRight, m_ignoreDynamicTiles);
	}

	// checks if the enemy falls would fall deeper than it can jump. 
	if (!m_collidesX && level.fallsDeep(bb, m_jumpHeight, m_isFacingRight, getDistanceToAbyss(), m_ignoreDynamicTiles)) {
		m_enemy->setAccelerationX(0.f);
		m_enemy->setVelocityX(0.f);
		m_enemy->setPositionX(oldPositionX);
		m_collidesX = true; // it kind of collides. this is used for the enemy if it shall wait.
	}

	// if the enemy collidesX but can't jump and is chasing, it waits for a certain time.
	if (m_enemy->getEnemyState() == EnemyState::Chasing && m_collidesX && !m_jumps) {
		m_enemy->setWaiting();
	}
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
