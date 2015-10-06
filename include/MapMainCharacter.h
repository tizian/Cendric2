#pragma once

#include "global.h"
#include "MovableGameObject.h"
#include "Map.h"
#include "InputController.h"
#include "Screen.h"
#include "CharacterCore.h"
#include "GUI/DialogueWindow.h"

class MapScreen;

// Cendric on the map
class MapMainCharacter : public MovableGameObject {
public:
	MapMainCharacter(Map* level);
	~MapMainCharacter();

	void load();
	void update(const sf::Time& frameTime) override;
	void checkCollisions(const sf::Vector2f& nextPosition);
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;

	void setCharacterCore(CharacterCore* core);


	float getConfiguredMaxVelocityY() const override;
	float getConfiguredMaxVelocityX() const override;
	GameObjectType getConfiguredType() const override;

private:
	const float WALK_ACCELERATION = 1500.0f;
	// handle input and calculate the next position
	void handleInput();
	// update animation based on the current velocity
	void updateAnimation();
	GameObjectState m_state;
	Map* m_map;
	CharacterCore* m_core;
};