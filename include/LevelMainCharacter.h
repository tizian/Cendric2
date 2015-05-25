#pragma once

#include "global.h"
#include "LevelMovableGameObject.h"
#include "Level.h"
#include "InputController.h"
#include "SpellManager.h"
#include "Screen.h"
#include "CharacterCore.h"

// Cendric in a level
class LevelMainCharacter : public LevelMovableGameObject
{
public:
	LevelMainCharacter(Level* level);
	~LevelMainCharacter();

	void load() override;
	void checkCollisions(const sf::Vector2f& nextPosition) override;

	void setCharacterCore(CharacterCore* core);
	
	float getConfiguredMaxVelocityY() const override;
	float getConfiguredMaxVelocityX() const override;
	GameObjectType getConfiguredType() const override;
	sf::Color getConfiguredDebugColor() const override;

protected:
	float getConfiguredDampingGroundPersS() const;
	// handle input and calculate the next position
	void handleInput() override;

private:
	// TODO load this map from configuration
	std::map<Key, SpellID> m_keyMap;
	CharacterCore* m_core;
};