#pragma once

#include "global.h"
#include "Map/MapMovableGameObject.h"
#include "World/MainCharacter.h"
#include "Map.h"
#include "InputController.h"
#include "Screens/Screen.h"
#include "CharacterCore.h"
#include "GUI/DialogueWindow.h"

class MapScreen;

// Cendric on the map
class MapMainCharacter final : public MapMovableGameObject, public MainCharacter {
public:
	MapMainCharacter(Map* level);
	~MapMainCharacter();

	void load();
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;
	void checkCollisions(const sf::Vector2f& nextPosition);
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;

	void updateFirst(const sf::Time& frameTime) override { MapMovableGameObject::updateFirst(frameTime); }
	void renderAfterForeground(sf::RenderTarget& target) override { MapMovableGameObject::renderAfterForeground(target); }
	void setDebugBoundingBox(const sf::Color& color) override { MapMovableGameObject::setDebugBoundingBox(color); }
	void setState(GameObjectState state) override { MapMovableGameObject::setState(state); }
	void setPosition(const sf::Vector2f& pos) override { MapMovableGameObject::setPosition(pos); }

	void setCharacterCore(CharacterCore* core);

	float getConfiguredMaxVelocityYUp() const override;
	float getConfiguredMaxVelocityYDown() const override;
	float getConfiguredMaxVelocityX() const override;
	GameObjectType getConfiguredType() const override;

private:
	std::string getSpritePath() const;
	const float WALK_ACCELERATION = 1500.0f;
	Key m_currentInput = Key::VOID;
	bool m_isCurrentInputActive = false;
	std::map<Key, sf::Vector2f> m_inputMap;
	// handle input and calculate the next position
	void handleInput();
	CharacterCore* m_core;
};