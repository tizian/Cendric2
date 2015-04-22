#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "Map.h"
#include "InputController.h"

// Cendric on the map
class MapMainCharacter : public MovableGameObject
{
public:
	MapMainCharacter(Map* level);
	~MapMainCharacter();

	void load() override;
	void update(sf::Time& frameTime) override;
	void checkCollisions(sf::Vector2f nextPosition) override;
	void calculateUnboundedVelocity(sf::Time& frameTime, sf::Vector2f& nextVel) override;

	const float getConfiguredMaxVelocityY() override;
	const float getConfiguredMaxVelocityX() override;
	GameObjectType getConfiguredType() const override;

private:
	const float WALK_ACCELERATION = 1500.0f;
	// choose a value between 0.9 for really slow halting and 1.0f for aprupt halting.
	const float DAMPING_PER_S = 0.999f;
	// handle input and calculate the next position
	void handleInput();
	// update animation based on the current velocity
	void updateAnimation();
	GameObjectState m_state;
	Map* m_map;
};