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
	calculateNextVelocity(frameTime, m_velocity);
	Vector2f position;
	calculateNextPosition(frameTime, position);
	setPosition(position);
	GameObject::update(frameTime);
}

void MovableGameObject::checkCollisions(sf::Vector2f nextPosition)
{
	//nop
}

void MovableGameObject::calculateNextPosition(sf::Time& frameTime, sf::Vector2f& nextPos)
{
	Vector2f position = getPosition();
	nextPos.x = position.x + m_velocity.x * frameTime.asSeconds();
	nextPos.y = position.y + m_velocity.y * frameTime.asSeconds();
}

void MovableGameObject::calculateNextVelocity(sf::Time& frameTime, sf::Vector2f& nextVel)
{
	nextVel.x = m_velocity.x + m_acceleration.x * frameTime.asSeconds();
	nextVel.y = m_velocity.y + m_acceleration.y * frameTime.asSeconds();
}