#pragma once

#include <vector>

#include "global.h"
#include "Screen.h"

#include "Enums/LevelDynamicTileID.h"
#include "Enums/EnemyID.h"

#include "Level/DynamicTiles/ChestTile.h"
#include "Level/DynamicTiles/LeverTile.h"
#include "Level/DynamicTiles/SwitchableTile.h"

#include "Structs/LevelData.h"

class Level;
class Enemy;
class LevelDynamicTile;

// helper class used by the level to load dynamic tiles, level items and enemies
class LevelLoader {
public:
	// loads dynamic tiles out of level data and adds them directly to the screen. An exception are dynamic chest tiles and levers.
	void loadDynamicTiles(LevelData& data, Screen* screen, Level* level) const;
	// loads chest tiles out of level data and adds them directly to the screen
	void loadChestTiles(LevelData& data, Screen* screen, Level* level) const;
	// loads lever tiles out of level data and adds them directly to the screen
	void loadLeverTiles(LevelData& data, Screen* screen, Level* level) const;
	// loads level items out of level data and adds them directly to the screen
	void loadLevelItems(LevelData& data, Screen* screen) const;
	// loads enemies out of level data and adds them directly to the screen
	void loadEnemies(LevelData& data, Screen* screen, Level* level) const;
	// loads lights out of level data and adds them directly to the screen
	void loadLights(LevelData& data, Screen* screen) const;
};