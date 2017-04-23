#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class DestructibleTile final : public LevelDynamicTile {
public:
	DestructibleTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Destructible; }

private:
	std::string getSpritePath() const override;
	std::string getSoundPath() const override;
	sf::Time m_crumblingTime = sf::seconds(0.4f);
};