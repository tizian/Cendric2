#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class IceTile : public LevelDynamicTile {
public:
	IceTile(Level* level) : LevelDynamicTile(level) {}
	void init() override;
	void load(int skinNr) override;
	void onHit(Spell* spell) override;

private:
};