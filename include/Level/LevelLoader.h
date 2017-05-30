#pragma once

#include "global.h"
#include "Screens/Screen.h"

#include "Enums/LevelDynamicTileID.h"
#include "Enums/EnemyID.h"

#include "Level/DynamicTiles/ChestLevelTile.h"
#include "Level/DynamicTiles/LeverTile.h"
#include "Level/DynamicTiles/SwitchableTile.h"

#include "Structs/LevelData.h"

class Level;
class Enemy;
class LevelDynamicTile;
class LevelScreen;

// helper class used by the level to load dynamic tiles, level items and enemies
class LevelLoader final {
public:
	void loadAfterMainChar(LevelData& data, LevelScreen* screen, Level* level) const;
	// loads lights out of level data and adds them directly to the screen
	void loadLights(LevelData& data, LevelScreen* screen) const;
	// loads dynamic tiles out of level data and adds them directly to the screen. 
	// An exeption are dynamic tiles that are loaded after the main char is loaded.
	void loadDynamicTiles(LevelData& data, LevelScreen* screen) const;

private:
	void loadDynamicTileData(std::vector<LevelDynamicTileData>& data, LevelScreen* screen) const;
	// loads level items out of level data and adds them directly to the screen
	void loadLevelItems(LevelData& data, LevelScreen* screen) const;
	// loads enemies out of level data and adds them directly to the screen
	void loadEnemies(LevelData& data, LevelScreen* screen, Level* level) const;
	// loads triggers out of level data and adds them directly to the screen
	void loadTriggers(LevelData& data, LevelScreen* screen) const;
};