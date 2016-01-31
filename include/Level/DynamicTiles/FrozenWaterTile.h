#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class SimulatedWaterTile;

class FrozenWaterTile : public LevelDynamicTile {
public:
	FrozenWaterTile(SimulatedWaterTile *waterTile, int waterTileIndex);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;

private:
	SimulatedWaterTile *m_waterTile;	// Pointer to its water tile, to unfreeze
	int m_waterTileIndex;				// Index of this subtile in its water tile
};