#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class DoorLevelTile : public LevelDynamicTile {
public:
	DoorLevelTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Door; }

private:
	std::string getSpritePath() const override;
};