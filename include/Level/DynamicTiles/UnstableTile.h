#pragma once

#include "global.h"
#include "Level/DynamicTiles/LevelMovableTile.h"

class UnstableTile : public virtual LevelMovableTile {
public:
	UnstableTile(LevelScreen* levelScreen);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void onHit(LevelMovableGameObject* mob) override;
	void update(const sf::Time& frameTime) override;

private:
	std::string getSpritePath() const override;
	void calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const override;

	const float GRAVITY_ACCELERATION = 1000.f;
	const float DAMPING_AIR = 0.7f;
	const float DAMPING_GROUND = 0.999f;

	// set to true if a mob stands on it
	bool m_isCritical = false;
	bool m_wasCritical = false;
	// set to true if the critical timer has timed out or a spell has activated the tile.
	bool m_isFalling = false;
	sf::Time m_criticalTime;
	sf::Time m_crumblingTime = sf::seconds(0.4f);
	static const sf::Time CRITICAL_TIME;
};