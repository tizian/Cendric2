#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class SwitchableTile : public LevelDynamicTile {
public:
	SwitchableTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	void init() override;
	void setInitialState(bool on);
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override { /*nop*/ };

	void switchTile();
};