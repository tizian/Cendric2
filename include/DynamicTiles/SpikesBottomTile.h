#pragma once

#include "global.h"
#include "DynamicTile.h"

class SpikesBottomTile : public DynamicTile {
public:
	SpikesBottomTile(Level* level) : DynamicTile(level) {}
	void init() override;
	void load(int skinNr) override;
	void onHit(LevelMovableGameObject* mob) override;
	void onHit(Spell* spell) override { /*nop*/ };
	void update(const sf::Time& frameTime) override;

private:
	const int DMG_PER_S = 50;
	sf::Time m_damageCooldown = sf::Time::Zero;
};