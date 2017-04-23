#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class IceTile final : public LevelDynamicTile {
public:
	IceTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Ice; }

private:
	std::string getSpritePath() const override;
};