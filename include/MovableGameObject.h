#include "stdafx.h"

const float GRAVITY_ACCELERATION = 600.0f;
const float MAX_VELOCITY_X = 1000.0f;
const float MAX_VELOCITY_Y = 1000.0f;
const float DAMPING = 0.9f;

// A movable game object with physics
class MovableGameObject : public GameObject
{
public:
	MovableGameObject();
	~MovableGameObject();

	void update(sf::Time& frameTime) override;
	void setAcceleration(sf::Vector2f& acceleration);
	void setAccelerationX(float accelerationX);
	void setAccelerationY(float accelerationY);
	void setVelocity(sf::Vector2f& velocity);
	void setVelocityX(float velocityX);
	void setVelocityY(float velocityY);
	void calculateNextPosition(sf::Time& frameTime, sf::Vector2f& nextPos);
	void calculateNextVelocity(sf::Time& frameTime, sf::Vector2f& nextVel);
	virtual void checkCollisions(sf::Vector2f nextPosition);

	sf::Vector2f& getVelocity();

private:
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
};