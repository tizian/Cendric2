#pragma once

#include "global.h"
#include "Level/DynamicTiles/LevelMovableTile.h"

class ShiftableTile : public virtual LevelMovableTile {
public:
	ShiftableTile(LevelScreen* levelScreen);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;

private:
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;

	const float GRAVITY_ACCELERATION = 1000.f;
	const float DAMPING_AIR = 0.7f;
	const float DAMPING_GROUND = 0.999f;

	float m_pushAcceleration = 0.f;

	sf::Time m_crumblingTime = sf::seconds(0.4f);
};