#pragma once

#include "global.h"
#include "DynamicTile.h"

class ChestTile : public DynamicTile
{
public:
	ChestTile() : DynamicTile() {}
	void init() override;
	void load(int skinNr) override;
	void onHit(Spell* spell) override;

private:
};