#pragma once

#include <vector>

#include "global.h"
#include "Screen.h"
#include "LevelReader.h"
#include "DynamicTile.h"
#include "DynamicTiles\WaterTile.h"
#include "DynamicTiles\IceTile.h"
#include "DynamicTiles\CrumblyBlockTile.h"
#include "LevelItem.h"
#include "LevelItems\CheeseLevelItem.h"

// helper class used by the level to load dynamic tiles, level items and enemies
class LevelLoader
{
public:
	// loads dynamic tiles out of level data and adds them directly to the screen
	void loadDynamicTiles(LevelData& data, Screen* screen);
	// loads level items out of level data and adds them directly to the screen
	void loadLevelItems(LevelData& data, Screen* screen);
	// loads enemies out of level data and adds them directly to the screen
	void loadEnemies(LevelData& data, Screen* screen);
};