#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "MovableGameObject.h"

enum class FallingTileState {
	Idle,
	Falling,
	Waiting,
	Returning
};

class FallingTile : public virtual LevelDynamicTile, public virtual MovableGameObject {
public:
	FallingTile(Level* level);

	void updateFirst(const sf::Time& frameTime) override { MovableGameObject::updateFirst(frameTime); }
	void renderAfterForeground(sf::RenderTarget& target) override { MovableGameObject::renderAfterForeground(target); }
	void setDebugBoundingBox(const sf::Color &debugColor) override { MovableGameObject::setDebugBoundingBox(debugColor); }

	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void setScreen(Screen* screen) override;
	void onHit(LevelMovableGameObject* mob) override;

	GameObjectType getConfiguredType() const override { return LevelDynamicTile::getConfiguredType(); }

private:
	void checkCollisions(const sf::Vector2f& nextPosition);

	GameObject* m_mainChar;
	const float GRAVITY_ACCELERATION = 1000.f;
	const float RETURN_VELOCITY = 80.f;
	const float AGGRO_DISTANCE = 100.f;
	const sf::Time WAITING_TIME = sf::seconds(2.f);

	FallingTileState m_tileState;
	sf::Time m_waitingTime = sf::Time::Zero;
};