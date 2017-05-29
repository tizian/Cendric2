#pragma once

#include "global.h"
#include "Level/DynamicTiles/LevelMovableTile.h"

// available skins:
// 0: default mushroom
// 1: default mushroom, shifted half a tile
// 2: unshiftable mushroom
// 3: unshiftable mushroom, shifted half a tile
class TrampolineTile final : public LevelMovableTile {
public:
	TrampolineTile(LevelScreen* levelScreen);
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Trampoline; }

	float getConfiguredMaxVelocityX() const override { return 200.f; }

private:
	std::string getSpritePath() const override;
	std::string getSoundPath() const override;
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;

	const float GRAVITY_ACCELERATION = 1000.f;
	const float DAMPING_AIR = 0.7f;
	const float DAMPING_GROUND = 0.999f;

	float m_pushAcceleration = 0.f;
	bool m_isShiftable;

	sf::Time m_crumblingTime;
	sf::Time m_jumpingTime;
	sf::FloatRect m_jumpingRegion;
};