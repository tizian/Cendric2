#pragma once

#include "global.h"
#include "Level/DynamicTiles/LevelMovableTile.h"

class TrampolineTile : public virtual LevelMovableTile {
public:
	TrampolineTile(LevelScreen* levelScreen);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void setPosition(const sf::Vector2f& pos) override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Trampoline; }

private:
	std::string getSpritePath() const override;
	std::string getSoundPath() const override;
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;

	const float GRAVITY_ACCELERATION = 1000.f;
	const float DAMPING_AIR = 0.7f;
	const float DAMPING_GROUND = 0.999f;

	sf::Time m_crumblingTime;
	sf::Time m_jumpingTime;
	sf::FloatRect m_jumpingRegion;
	sf::Sound m_sound;
};