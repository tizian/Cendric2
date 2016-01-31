#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class DestructibleTile : public LevelDynamicTile {
public:
	DestructibleTile(Level* level) : LevelDynamicTile(level) {}
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;

private:
	sf::Time m_crumblingTime = sf::seconds(0.4f);
};