#pragma once

#include "global.h"
#include "DynamicTile.h"

class SpikesTopTile : public DynamicTile
{
public:
	void load() override;
	void onHit(Spell* spell) override;
	void onHit(LevelMovableGameObject* mob) override;
	void update(const sf::Time& frameTime) override;

private:
	const int DMG_PER_S = 30;
	sf::Time m_damageCooldown = sf::Time::Zero;
};