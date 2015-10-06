#pragma once

#include "global.h"
#include "DynamicTile.h"
#include "MovableGameObject.h"

class ShiftableBlockTile : public DynamicTile, public MovableGameObject {
public:
	ShiftableBlockTile(Level* level);
	void init() override;
	void load(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;

	GameObjectType getConfiguredType() const override;

private:
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;
	void checkCollisions(const sf::Vector2f& nextPosition);

	const float PUSH_ACCELERATION_X = 100.f;
	const float GRAVITY_ACCELERATION = 1000.f;
	const float DAMPING_AIR = 0.7f;
	const float DAMPING_GROUND = 0.999f;

	float m_pushAcceleration = 0.f;
};