#pragma once

#include <vector>

#include "global.h"
#include "Screen.h"
#include "DynamicTiles/WaterTile.h"
#include "DynamicTiles/IceTile.h"
#include "DynamicTiles/CrumblyBlockTile.h"
#include "DynamicTiles/TorchTile.h"
#include "DynamicTiles/ChestTile.h"

#include "Enums/DynamicTileID.h"
#include "Enums/LevelItemID.h"
#include "Enums/ItemID.h"
#include "Enums/EnemyID.h"

#include "Structs/LevelData.h"
#include "Structs/LevelItemBean.h"

class Level;

// helper class used by the level to load dynamic tiles, level items and enemies
class LevelLoader
{
public:
	// loads dynamic tiles out of level data and adds them directly to the screen
	void loadDynamicTiles(LevelData& data, Screen* screen) const;
	// loads level items out of level data and adds them directly to the screen
	void loadLevelItems(LevelData& data, Screen* screen) const;
	// loads enemies out of level data and adds them directly to the screen
	void loadEnemies(LevelData& data, Screen* screen, Level* level) const;
};