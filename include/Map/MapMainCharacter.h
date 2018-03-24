#pragma once

#include "global.h"
#include "Map/MapMovableGameObject.h"
#include "World/MainCharacter.h"
#include "Map.h"
#include "CharacterCore.h"
#include "GUI/DialogueWindow.h"

class MapScreen;

// Cendric on the map
class MapMainCharacter final : public MapMovableGameObject, public MainCharacter {
public:
	MapMainCharacter(Map* level, Screen* screen);
	~MapMainCharacter();

	void load();
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;
	void checkCollisions(const sf::Vector2f& nextPosition);
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;
	void boundVelocity(sf::Vector2f& vel) const override;

	void updateFirst(const sf::Time& frameTime) override { MapMovableGameObject::updateFirst(frameTime); }
	void renderAfterForeground(sf::RenderTarget& target) override { MapMovableGameObject::renderAfterForeground(target); }
	void setDebugBoundingBox(const sf::Color& color) override { MapMovableGameObject::setDebugBoundingBox(color); }
	void setState(GameObjectState state) override { MapMovableGameObject::setState(state); }
	void setPosition(const sf::Vector2f& pos) override { MapMovableGameObject::setPosition(pos); }

	void setCharacterCore(CharacterCore* core);

	float getConfiguredMaxVelocityX() const override;
	GameObjectType getConfiguredType() const override;
	const sf::Vector2f& getPreviousPosition() const;

private:
	std::string getSpritePath() const;
	const float WALK_ACCELERATION = 1500.0f;
	// handle input and calculate the next position
	void handleInput();
	CharacterCore* m_core;
	sf::Vector2f m_previousFramePosition;
};