#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "Map.h"
#include "InputController.h"

const float MAP_WALK_ACCELERATION = 1500.0f;

// Cendric on that map
class MapMainCharacter : public MovableGameObject
{
public:
	MapMainCharacter(Map* level);
	~MapMainCharacter();

	void load() override;
	void update(sf::Time& frameTime) override;
	void checkCollisions(sf::Vector2f nextPosition) override;
	void calculateNextVelocity(sf::Time& frameTime, sf::Vector2f& nextVel) override;

private:
	// handle input and calculate the next position
	void handleInput();
	// update animation based on the current velocity
	void updateAnimation();
	GameObjectState m_state;
	sf::Vector2f m_nextPosition;
	Map* m_map;

	bool m_isFacingUp;
	bool m_isFacingRight;
};