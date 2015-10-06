#pragma once

#include <vector>

#include "global.h"
#include "Screen.h"
#include "DynamicTiles/IceTile.h"
#include "DynamicTiles/CrumblyBlockTile.h"
#include "DynamicTiles/TorchTile.h"
#include "DynamicTiles/ChestTile.h"
#include "DynamicTiles/SpikesBottomTile.h"
#include "DynamicTiles/SpikesTopTile.h"
#include "DynamicTiles/SimulatedWaterTile.h"
#include "DynamicTiles/ShiftableBlockTile.h"
#include "DynamicTiles/CheckpointTile.h"

#include "Enums/DynamicTileID.h"
#include "Enums/EnemyID.h"

#include "Structs/LevelData.h"

class Level;

// helper class used by the level to load dynamic tiles, level items and enemies
class LevelLoader {
public:
	// loads dynamic tiles out of level data and adds them directly to the screen. An exception are dynamic chest tiles.
	void loadDynamicTiles(LevelData& data, Screen* screen, Level* level) const;
	// loads chest tiles out of level data and adds them directly to the screen
	void loadChestTiles(LevelData& data, Screen* screen, Level* level) const;
	// loads level items out of level data and adds them directly to the screen
	void loadLevelItems(LevelData& data, Screen* screen) const;
	// loads enemies out of level data and adds them directly to the screen
	void loadEnemies(LevelData& data, Screen* screen, Level* level) const;
	// loads lights out of level data and adds them directly to the screen
	void loadLights(LevelData& data, Screen* screen) const;
};