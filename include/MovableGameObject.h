#pragma once

#include "global.h"
#include "GameObject.h"

// A movable game object with physics.  Abstract class
class MovableGameObject : virtual public GameObject
{
public:
	MovableGameObject() : GameObject() {}
	virtual ~MovableGameObject() {}

	void update(const sf::Time& frameTime) override;
	void calculateNextPosition(const sf::Time& frameTime, sf::Vector2f& nextPos) const;
	void calculateNextVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const;
	virtual void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const;

	void setAcceleration(const sf::Vector2f& acceleration);
	void setAccelerationX(float accelerationX);
	void setAccelerationY(float accelerationY);
	void setVelocity(const sf::Vector2f& velocity);
	void setVelocityX(float velocityX);
	void setVelocityY(float velocityY);
	
	virtual float getConfiguredMaxVelocityY() const;
	virtual float getConfiguredMaxVelocityX() const;
	const sf::Vector2f& getVelocity() const;
	const sf::Vector2f& getAcceleration() const;

private:
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
	void boundVelocity(sf::Vector2f& vel) const;
};