#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "MovableGameObject.h"

enum class JumpingTileState {
	Idle,
	Falling,
	Waiting,
	Returning
};

class JumpingTile : public virtual LevelDynamicTile, public virtual MovableGameObject {
public:
	JumpingTile(LevelScreen* levelScreen);

	void updateFirst(const sf::Time& frameTime) override { MovableGameObject::updateFirst(frameTime); }
	void renderAfterForeground(sf::RenderTarget& target) override { MovableGameObject::renderAfterForeground(target); }
	void setDebugBoundingBox(const sf::Color& debugColor) override { MovableGameObject::setDebugBoundingBox(debugColor); }

	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void onHit(LevelMovableGameObject* mob) override;

	GameObjectType getConfiguredType() const override { return LevelDynamicTile::getConfiguredType(); }

private:
	void checkCollisions(const sf::Vector2f& nextPosition);

	const float GRAVITY_ACCELERATION = 1000.f;
	const float RETURN_VELOCITY = 80.f;
	const float AGGRO_DISTANCE = 200.f;
	const sf::Time WAITING_TIME = sf::seconds(2.f);

	JumpingTileState m_tileState;
	sf::Time m_waitingTime = sf::Time::Zero;
};