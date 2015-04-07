#include "MovableGameObject.h"

void MovableGameObject::update(sf::Time& frameTime)
{
	sf::Vector2f position;
	calculateNextPosition(frameTime, position);
	setPosition(position);
	calculateNextVelocity(frameTime, m_velocity);
	GameObject::update(frameTime);
}

void MovableGameObject::checkCollisions(sf::Vector2f nextPosition)
{
	//nop
}

void MovableGameObject::calculateNextPosition(sf::Time& frameTime, sf::Vector2f& nextPos)
{
	sf::Vector2f position = getPosition();
	sf::Vector2f nextVel;
	calculateNextVelocity(frameTime, nextVel);
	nextPos.x = position.x + nextVel.x * frameTime.asSeconds();
	nextPos.y = position.y + nextVel.y * frameTime.asSeconds();
}

void MovableGameObject::calculateUnboundedVelocity(sf::Time& frameTime, sf::Vector2f& nextVel) 
{
	nextVel.x = (m_velocity.x + m_acceleration.x * frameTime.asSeconds());
	nextVel.y = (m_velocity.y + m_acceleration.y * frameTime.asSeconds());
}

void MovableGameObject::boundVelocity(sf::Vector2f& vel)
{
	// check bounds
	if (vel.x > getConfiguredMaxVelocityX()) vel.x = getConfiguredMaxVelocityX();
	if (vel.x < -getConfiguredMaxVelocityX()) vel.x = -getConfiguredMaxVelocityX();
	if (vel.y > getConfiguredMaxVelocityY()) vel.y = getConfiguredMaxVelocityY();
	if (vel.y < -getConfiguredMaxVelocityY()) vel.y = -getConfiguredMaxVelocityY();
}

void MovableGameObject::calculateNextVelocity(sf::Time& frameTime, sf::Vector2f& nextVel)
{
	calculateUnboundedVelocity(frameTime, nextVel);
	boundVelocity(nextVel);	
}

void MovableGameObject::setAcceleration(const sf::Vector2f& acceleration)
{
	m_acceleration = acceleration;
}

void MovableGameObject::setAccelerationX(float accelerationX)
{
	m_acceleration.x = accelerationX;
}

void MovableGameObject::setAccelerationY(float accelerationY)
{
	m_acceleration.y = accelerationY;
}

void MovableGameObject::setVelocity(sf::Vector2f& velocity)
{
	m_velocity = velocity;
}

void MovableGameObject::setVelocityX(float velocityX)
{
	m_velocity.x = velocityX;
}

void MovableGameObject::setVelocityY(float velocityY)
{
	m_velocity.y = velocityY;
}

sf::Vector2f& MovableGameObject::getVelocity()
{
	return m_velocity;
}

sf::Vector2f& MovableGameObject::getAcceleration()
{
	return m_acceleration;
}