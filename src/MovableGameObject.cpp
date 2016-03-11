#include "MovableGameObject.h"
#include "Level/DynamicTiles/MovingTile.h"

MovableGameObject::MovableGameObject() : AnimatedGameObject() {
}

void MovableGameObject::updateFirst(const sf::Time& frameTime) {
	updateRelativeVelocity(frameTime);
}

void MovableGameObject::update(const sf::Time& frameTime) {
	sf::Vector2f position;
	calculateNextPosition(frameTime, position);
	setPosition(position);
	calculateNextVelocity(frameTime, m_velocity);
	AnimatedGameObject::update(frameTime);
	if (m_debugInfo) {
		m_debugInfo->setString("x: " + std::to_string(getPosition().x) + " y: " + std::to_string(getPosition().y));
		m_debugInfo->setPosition(getPosition() + sf::Vector2f(0.f, -30.f));
	}
}

void MovableGameObject::setDebugBoundingBox(const sf::Color &debugColor) {
	if (!g_resourceManager->getConfiguration().isDebugRendering) return;
	GameObject::setDebugBoundingBox(debugColor);
	delete m_debugInfo;
	m_debugInfo = new BitmapText();
	m_debugInfo->setColor(COLOR_BAD);
}

void MovableGameObject::renderAfterForeground(sf::RenderTarget& target) {
	GameObject::renderAfterForeground(target);
	if (m_debugInfo) {
		target.draw(*m_debugInfo);
	}
}

void MovableGameObject::lockRelativeVelocityX() {
	m_isLockedRelativeVelocityX = true; 
}

void MovableGameObject::lockRelativeVelocityY() {
	m_isLockedRelativeVelocityY = true;
} 

void MovableGameObject::updateRelativeVelocity(const sf::Time& frameTime) {
	if (m_movingParent == nullptr) {
		m_isLockedRelativeVelocityX = false;
		m_isLockedRelativeVelocityY = false;
		return;
	}
	sf::Vector2f nextPos = getPosition();
	if (!m_isLockedRelativeVelocityX) nextPos.x += m_movingParent->getRelativeVelocity().x * frameTime.asSeconds();
	if (!m_isLockedRelativeVelocityY) nextPos.y += m_movingParent->getRelativeVelocity().y * frameTime.asSeconds();
	setPosition(nextPos);
	m_isLockedRelativeVelocityX = false;
	m_isLockedRelativeVelocityY = false;
}

void MovableGameObject::calculateNextPosition(const sf::Time& frameTime, sf::Vector2f& nextPos) const {
	sf::Vector2f nextVel;
	calculateNextVelocity(frameTime, nextVel); 
	sf::Vector2f currentPos = getPosition();
	nextPos.x = currentPos.x + nextVel.x * frameTime.asSeconds();
	nextPos.y = currentPos.y + nextVel.y * frameTime.asSeconds();
}

void MovableGameObject::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	nextVel.x = (m_velocity.x + m_acceleration.x * frameTime.asSeconds());
	nextVel.y = (m_velocity.y + m_acceleration.y * frameTime.asSeconds());
}

void MovableGameObject::boundVelocity(sf::Vector2f& vel) const {
	// check bounds
	if (vel.x > getConfiguredMaxVelocityX()) vel.x = getConfiguredMaxVelocityX();
	if (vel.x < -getConfiguredMaxVelocityX()) vel.x = -getConfiguredMaxVelocityX();
	if (vel.y > getConfiguredMaxVelocityYDown()) vel.y = getConfiguredMaxVelocityYDown();
	if (vel.y < -getConfiguredMaxVelocityYUp()) vel.y = -getConfiguredMaxVelocityYUp();
}

void MovableGameObject::calculateNextVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	calculateUnboundedVelocity(frameTime, nextVel);
	boundVelocity(nextVel);
}

void MovableGameObject::setAcceleration(const sf::Vector2f& acceleration) {
	m_acceleration = acceleration;
}

void MovableGameObject::setAccelerationX(float accelerationX) {
	m_acceleration.x = accelerationX;
}

void MovableGameObject::setAccelerationY(float accelerationY) {
	m_acceleration.y = accelerationY;
}

void MovableGameObject::addAccelerationX(float accelerationX) {
	m_acceleration.x += accelerationX;
}

void MovableGameObject::addAccelerationY(float accelerationY) {
	m_acceleration.y += accelerationY;
}

void MovableGameObject::setVelocity(const sf::Vector2f& velocity) {
	m_velocity = velocity;
}

void MovableGameObject::setMovingParent(MovingTile* parent) {
	m_movingParent = parent;
}

void MovableGameObject::setVelocityX(float velocityX) {
	m_velocity.x = velocityX;
}

void MovableGameObject::setVelocityY(float velocityY) {
	m_velocity.y = velocityY;
}

float MovableGameObject::getConfiguredMaxVelocityX() const {
	return 1000.0f;
}

float MovableGameObject::getConfiguredMaxVelocityYUp() const {
	return 1000.0f;
}

float MovableGameObject::getConfiguredMaxVelocityYDown() const {
	return 1000.0f;
}

const sf::Vector2f& MovableGameObject::getVelocity() const {
	return m_velocity;
}

const sf::Vector2f& MovableGameObject::getAcceleration() const {
	return m_acceleration;
}

MovingTile* MovableGameObject::getMovingParent() const {
	return m_movingParent;
}