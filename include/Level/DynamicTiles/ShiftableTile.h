#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "MovableGameObject.h"

class ShiftableTile : public virtual LevelDynamicTile, public virtual MovableGameObject {
public:
	ShiftableTile(Level* level);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void updateFirst(const sf::Time& frameTime) override { MovableGameObject::updateFirst(frameTime); }

	void renderAfterForeground(sf::RenderTarget& target) override { MovableGameObject::renderAfterForeground(target); }
	void setDebugBoundingBox(const sf::Color &debugColor) override { MovableGameObject::setDebugBoundingBox(debugColor); }

	GameObjectType getConfiguredType() const override;

private:
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;
	void checkCollisions(const sf::Vector2f& nextPosition);

	const float GRAVITY_ACCELERATION = 1000.f;
	const float DAMPING_AIR = 0.7f;
	const float DAMPING_GROUND = 0.999f;

	float m_pushAcceleration = 0.f;
};