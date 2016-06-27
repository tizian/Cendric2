#pragma once

#include <vector>

#include "global.h"
#include "Screens/Screen.h"

#include "Enums/LevelDynamicTileID.h"
#include "Enums/EnemyID.h"

#include "Level/DynamicTiles/ChestTile.h"
#include "Level/DynamicTiles/LeverTile.h"
#include "Level/DynamicTiles/SwitchableTile.h"

#include "Structs/LevelData.h"

class Level;
class Enemy;
class LevelDynamicTile;
class LevelScreen;

// helper class used by the level to load dynamic tiles, level items and enemies
class LevelLoader {
public:
	void loadAfterMainChar(LevelData& data, LevelScreen* screen, Level* level) const;
	// loads lights out of level data and adds them directly to the screen
	void loadLights(LevelData& data, LevelScreen* screen) const;
	// loads dynamic tiles out of level data and adds them directly to the screen. 
	// An exeption are dynamic tiles that are loaded after the main char is loaded.
	void loadDynamicTiles(LevelData& data, LevelScreen* screen) const;

private:
	// loads chest tiles out of level data and adds them directly to the screen
	void loadChestTiles(LevelData& data, LevelScreen* screen) const;
	// loads modifier tiles out of level data and adds them directly to the screen
	void loadModifierTiles(LevelData& data, LevelScreen* screen) const;
	// loads jumping tiles out of level data and adds them directly to the screen
	void loadJumpingTiles(LevelData& data, LevelScreen* screen) const;
	// loads sign tiles out of level data and adds them directly to the screen
	void loadSignTiles(LevelData& data, LevelScreen* screen) const;
	// loads ladder tiles out of level data and adds them directly to the screen
	void loadLadderTiles(LevelData& data, LevelScreen* screen) const;
	// loads lever tiles (levers, moving tiles, switchable tiles) out of level data and adds them directly to the screen
	void loadLeverTiles(LevelData& data, LevelScreen* screen) const;
	// loads level items out of level data and adds them directly to the screen
	void loadLevelItems(LevelData& data, LevelScreen* screen) const;
	// loads enemies out of level data and adds them directly to the screen
	void loadEnemies(LevelData& data, LevelScreen* screen, Level* level) const;
	// loads triggers out of level data and adds them directly to the screen
	void loadTriggers(LevelData& data, LevelScreen* screen) const;
};