#pragma once

#include "global.h"
#include "GameObject.h"

// A movable game object with physics.  Abstract class
class MovableGameObject : public GameObject
{
public:

	void update(sf::Time& frameTime) override;
	void setAcceleration(const sf::Vector2f& acceleration);
	void setAccelerationX(float accelerationX);
	void setAccelerationY(float accelerationY);
	void setVelocity(sf::Vector2f& velocity);
	void setVelocityX(float velocityX);
	void setVelocityY(float velocityY);
	void calculateNextPosition(sf::Time& frameTime, sf::Vector2f& nextPos);
	void calculateNextVelocity(sf::Time& frameTime, sf::Vector2f& nextVel);
	virtual void calculateUnboundedVelocity(sf::Time& frameTime, sf::Vector2f& nextVel);
	virtual void checkCollisions(sf::Vector2f nextPosition);

	// configurable values, implemented by subclasses.
	virtual const float getConfiguredMaxVelocityY() = 0;
	virtual const float getConfiguredMaxVelocityX() = 0;

	sf::Vector2f& getVelocity();
	sf::Vector2f& getAcceleration();

private:
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
	void boundVelocity(sf::Vector2f& vel);
};