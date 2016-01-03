#pragma once

#include "global.h"
#include "Map/MapMovableGameObject.h"
#include "Map.h"
#include "InputController.h"
#include "Screen.h"
#include "CharacterCore.h"
#include "GUI/DialogueWindow.h"

class MapScreen;

// Cendric on the map
class MapMainCharacter : virtual public MapMovableGameObject {
public:
	MapMainCharacter(Map* level);
	~MapMainCharacter();

	void load();
	void update(const sf::Time& frameTime) override;
	void checkCollisions(const sf::Vector2f& nextPosition);
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;

	void setCharacterCore(CharacterCore* core);


	float getConfiguredMaxVelocityYUp() const override;
	float getConfiguredMaxVelocityYDown() const override;
	float getConfiguredMaxVelocityX() const override;
	GameObjectType getConfiguredType() const override;

private:
	const float WALK_ACCELERATION = 1500.0f;
	// handle input and calculate the next position
	void handleInput();
	GameObjectState m_state;
	CharacterCore* m_core;
};