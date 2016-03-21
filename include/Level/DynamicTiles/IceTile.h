#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class IceTile : public LevelDynamicTile {
public:
	IceTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
};