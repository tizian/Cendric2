#pragma once

#include "global.h"
#include "AnimatedGameObject.h"

// A movable game object with physics.  Abstract class
class MovableGameObject : virtual public AnimatedGameObject {
public:
	MovableGameObject();
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
	
	const sf::Vector2f& getVelocity() const;
	const sf::Vector2f& getAcceleration() const;

protected:
	virtual float getConfiguredMaxVelocityYUp() const;
	virtual float getConfiguredMaxVelocityYDown() const;
	virtual float getConfiguredMaxVelocityX() const;
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
	void boundVelocity(sf::Vector2f& vel) const;
};