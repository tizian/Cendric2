#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "Level.h"
#include "InputController.h"

const float WALK_ACCELERATION = 1500.0f;
const float JUMP_VEL_Y = -600.0f;

// Cendric
class MainCharacter : public MovableGameObject
{
public:
	MainCharacter(Level* level);
	~MainCharacter();

	void load() override;
	void update(sf::Time& frameTime) override;
	void checkCollisions(sf::Vector2f nextPosition) override;
	void calculateNextVelocity(sf::Time& frameTime, sf::Vector2f& nextVel) override;	

private:
	// handle input and calculate the next position
	void handleInput();
	// update animation based on the current velocity + grounded
	void updateAnimation();
	bool m_isFacingRight;
	bool m_nextIsFacingRight;
	GameObjectState m_state;
	bool m_isGrounded;
	sf::Vector2f m_nextPosition;
	Level* m_level;
};