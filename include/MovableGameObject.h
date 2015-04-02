#include "stdafx.h"

const float GRAVITY_ACCELERATION = 700.0f;
const float MAX_VELOCITY_X = 200.0f;
const float MAX_VELOCITY_Y = 700.0f;
// choose a value between 0.9 for really slow halting and 1.0f for aprupt halting.
const float DAMPING_GROUND_PER_S = 0.999f;
const float DAMPING_AIR_PER_S = 0.97f;

// A movable game object with physics
class MovableGameObject : public GameObject
{
public:
	MovableGameObject();
	~MovableGameObject();

	void update(sf::Time& frameTime) override;
	void setAcceleration(const sf::Vector2f& acceleration);
	void setAccelerationX(float accelerationX);
	void setAccelerationY(float accelerationY);
	void setVelocity(sf::Vector2f& velocity);
	void setVelocityX(float velocityX);
	void setVelocityY(float velocityY);
	void calculateNextPosition(sf::Time& frameTime, sf::Vector2f& nextPos);
	virtual void calculateNextVelocity(sf::Time& frameTime, sf::Vector2f& nextVel);
	virtual void checkCollisions(sf::Vector2f nextPosition);

	sf::Vector2f& getVelocity();

private:
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
};