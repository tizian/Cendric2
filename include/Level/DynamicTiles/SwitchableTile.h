#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class SwitchableTile : public LevelDynamicTile {
public:
	SwitchableTile(Level* level) : LevelDynamicTile(level) {}
	void init() override;
	void setInitialState(bool on);
	void load(int skinNr) override;
	void onHit(Spell* spell) override { /*nop*/ };

	void switchTile();
};