#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class SpikesTopTile : public LevelDynamicTile {
public:
	SpikesTopTile(Level* level) : LevelDynamicTile(level) {}
	void init() override;
	void load(int skinNr) override;
	void onHit(LevelMovableGameObject* mob) override;
	void onHit(Spell* spell) override { /*nop*/ };
	void update(const sf::Time& frameTime) override;

private:
	const int DMG_PER_S = 300;
	sf::Time m_damageCooldown = sf::Time::Zero;
};