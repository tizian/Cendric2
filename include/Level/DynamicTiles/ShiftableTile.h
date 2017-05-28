#pragma once

#include "global.h"
#include "Level/DynamicTiles/LevelMovableTile.h"

class ShiftableTile final : public LevelMovableTile {
public:
	ShiftableTile(LevelScreen* levelScreen);
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Shiftable; }

	float getConfiguredMaxVelocityX() const override { return 200.f; }

private:
	std::string getSpritePath() const override;
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;

	const float GRAVITY_ACCELERATION = 1000.f;
	const float DAMPING_AIR = 0.7f;
	const float DAMPING_GROUND = 0.999f;

	float m_pushAcceleration = 0.f;

	sf::Time m_crumblingTime = sf::seconds(0.4f);
};