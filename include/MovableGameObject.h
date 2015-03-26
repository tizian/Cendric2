#include "stdafx.h"

const float GRAVITY_ACCELERATION = 800.0f;

// A movable game object with physics
class MovableGameObject : public GameObject
{
public:
	MovableGameObject();
	~MovableGameObject();

	void update(sf::Time& frameTime) override;
	void calculateNextPosition(sf::Time& frameTime, sf::Vector2f& nextPos);
	void calculateNextVelocity(sf::Time& frameTime, sf::Vector2f& nextVel);
	virtual void checkCollisions(sf::Vector2f nextPosition);

private:
	sf::Vector2f m_velocity;
	sf::Vector2f m_acceleration;
};