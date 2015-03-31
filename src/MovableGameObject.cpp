#include "stdafx.h"

MovableGameObject::MovableGameObject()
{
	m_acceleration.y = GRAVITY_ACCELERATION;
}


MovableGameObject::~MovableGameObject()
{
}

void MovableGameObject::update(sf::Time& frameTime)
{
	Vector2f position;
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
	Vector2f position = getPosition();
	Vector2f nextVel;
	calculateNextVelocity(frameTime, nextVel);
	nextPos.x = position.x + nextVel.x * frameTime.asSeconds();
	nextPos.y = position.y + nextVel.y * frameTime.asSeconds();
}

void MovableGameObject::calculateNextVelocity(sf::Time& frameTime, sf::Vector2f& nextVel)
{
	nextVel.x = (m_velocity.x + m_acceleration.x * frameTime.asSeconds()) * (m_velocity.y == 0.0f ? DAMPING_GROUND : DAMPING_AIR);
	nextVel.y = m_velocity.y + m_acceleration.y * frameTime.asSeconds();

	// check bounds
	if (nextVel.x > MAX_VELOCITY_X) nextVel.x = MAX_VELOCITY_X;
	if (nextVel.x < -MAX_VELOCITY_X) nextVel.x = -MAX_VELOCITY_X;
	if (nextVel.y > MAX_VELOCITY_Y) nextVel.y = MAX_VELOCITY_Y;
	if (nextVel.y < -MAX_VELOCITY_Y) nextVel.y = -MAX_VELOCITY_Y;
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