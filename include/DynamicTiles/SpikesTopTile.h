#pragma once

#include "global.h"
#include "DynamicTile.h"

class SpikesTopTile : public DynamicTile {
public:
	SpikesTopTile(Level* level) : DynamicTile(level) {}
	void init() override;
	void load(int skinNr) override;
	void onHit(LevelMovableGameObject* mob) override;
	void onHit(Spell* spell) override { /*nop*/ };
	void update(const sf::Time& frameTime) override;

private:
	const int DMG_PER_S = 30;
	sf::Time m_damageCooldown = sf::Time::Zero;
};