#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class FluidTile;

class FrozenWaterTile : public LevelDynamicTile {
public:
	FrozenWaterTile(FluidTile *waterTile, int waterTileIndex);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void setDisposed() override;

private:
	FluidTile *m_fluidTile;			// Pointer to its fluid tile, to unfreeze
	int m_fluidTileIndex;			// Index of this subtile in its fluid tile
};