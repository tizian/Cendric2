#include "Level/EnemyBehavior/MovingBehavior.h"
#include "Screens/LevelScreen.h"

MovingBehavior::MovingBehavior(LevelMovableGameObject* mob) {
	m_mob = mob;
	m_mainChar = dynamic_cast<LevelScreen*>(mob->getScreen())->getMainCharacter();
}

void MovingBehavior::update(const sf::Time& frameTime) {
	handleMovementInput();
	
	sf::Vector2f nextPosition;
	m_mob->calculateNextPosition(frameTime, nextPosition);
	checkCollisions(nextPosition);

	// update animation
	GameObject::updateTime(m_fightAnimationTime, frameTime);
	updateAnimation();
}

void MovingBehavior::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	// distinguish damping in the air and at the ground
	float dampingPerSec = (m_mob->getVelocity().y == 0.0f) ? m_dampingGroundPerS : m_dampingAirPerS;
	// don't damp when there is active acceleration 
	if (m_mob->getAcceleration().x != 0.0f) dampingPerSec = 0;
	nextVel.x = (m_mob->getVelocity().x + m_mob->getAcceleration().x * frameTime.asSeconds()) * pow(1 - dampingPerSec, frameTime.asSeconds());
	nextVel.y = m_mob->getVelocity().y + m_mob->getAcceleration().y * frameTime.asSeconds();
}

void MovingBehavior::setFightAnimation() {
	m_fightAnimationTime = m_configuredFightAnimationTime;
}

void MovingBehavior::flipGravity() {
	m_isFlippedGravity = !m_isFlippedGravity;
}

void MovingBehavior::setGravityScale(float scale) {
	m_gravity = scale * m_configuredGravity;
	m_maxVelocityYDown = scale * m_configuredMaxVelocityYDown;
}

void MovingBehavior::setMaxXVelocityScale(float scale) {
	m_maxVelocityX = scale * m_configuredMaxVelocityX;
}

void MovingBehavior::setIgnoreDynamicTiles(bool value) {
	m_ignoreDynamicTiles = value;
}

void MovingBehavior::setWalkAcceleration(float acceleration) {
	m_walkAcceleration = acceleration;
}

void MovingBehavior::setGravityAcceleration(float acceleration) {
	m_configuredGravity = acceleration;
	m_gravity = acceleration;
}

float MovingBehavior::getGravity() const {
	return m_gravity;
}

bool MovingBehavior::isFacingRight() const {
	return m_isFacingRight;
}

bool MovingBehavior::isUpsideDown() const {
	return m_isFlippedGravity;
}

void MovingBehavior::setFightAnimationTime(const sf::Time& fightAnimationTIme) {
	m_configuredFightAnimationTime = fightAnimationTIme;
}

void MovingBehavior::setDampingGroundPerS(float damping) {
	m_dampingGroundPerS = damping;
}

void MovingBehavior::setDampingAirPerS(float damping) {
	m_dampingAirPerS = damping;
}

void MovingBehavior::setMaxVelocityX(float vel) {
	m_configuredMaxVelocityX = vel;
	m_maxVelocityX = vel;
}

void MovingBehavior::setMaxVelocityYUp(float vel) {
	m_configuredMaxVelocityYUp = vel;
	m_maxVelocityYUp = vel;
}

void MovingBehavior::setMaxVelocityYDown(float vel) {
	m_configuredMaxVelocityYDown = vel;
	m_maxVelocityYDown = vel;
}

float MovingBehavior::getMaxVelocityX() const {
	return m_maxVelocityX;
}

float MovingBehavior::getMaxVelocityYDown() const {
	return m_maxVelocityYDown;
}

float MovingBehavior::getMaxVelocityYUp() const {
	return m_maxVelocityYUp;
}
