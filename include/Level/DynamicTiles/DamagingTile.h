#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class DamagingTile : public LevelDynamicTile {
public:
	DamagingTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(LevelMovableGameObject* mob) override;
	void onHit(Spell* spell) override { /*nop*/ };
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Damaging; }

private:
	std::string getSpritePath() const override;
};