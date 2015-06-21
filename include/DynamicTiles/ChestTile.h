#pragma once

#include "global.h"
#include "DynamicTile.h"

class ChestTile : public DynamicTile
{
public:
	void load(int skinNr) override;
	void onHit(Spell* spell) override;

private:
};