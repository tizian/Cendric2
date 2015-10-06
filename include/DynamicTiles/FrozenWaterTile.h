#pragma once

#include "global.h"
#include "DynamicTile.h"

class SimulatedWaterTile;

class FrozenWaterTile : public DynamicTile {
public:
	FrozenWaterTile(SimulatedWaterTile *waterTile, int waterTileIndex);
	void init() override;
	void load(int skinNr) override;
	void onHit(Spell* spell) override;

private:
	SimulatedWaterTile *m_waterTile;	// Pointer to its water tile, to unfreeze
	int m_waterTileIndex;				// Index of this subtile in its water tile
};