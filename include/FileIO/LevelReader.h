#pragma once

#include "global.h"
#include "FileIO/Reader.h"
#include "BackgroundLayer.h"
#include "DynamicTile.h"
#include "LevelItem.h"
#include "Enemy.h"
#include "Logger.h"

#include "Structs/LevelData.h"

class LevelReader : public Reader
{
public:
	LevelReader();
	~LevelReader();

	bool readLevel(char* fileName, LevelData& data) const;

private:
	void initMaps();

	bool readLevelName(char* start, char* end, LevelData& data) const;
	bool readTilesetPath(char* start, char* end, LevelData& data) const;
	bool readMapSize(char* start, char* end, LevelData& data) const;
	bool readTileSize(char* start, char* end, LevelData& data) const;
	bool readLayerBackground(char* start, char* end, LevelData& data) const;
	bool readLayerCollidable(char* start, char* end, LevelData& data) const;
	bool readLayerTilesBackground(char* start, char* end, LevelData& data) const;
	bool readLayerTilesForeground(char* start, char* end, LevelData& data) const;
	bool readLayerDynamicTiles(char* start, char* end, LevelData& data) const;
	bool readLayerChestTiles(char* start, char* end, LevelData& data) const;
	bool readLayerLevelItems(char* start, char* end, LevelData& data) const;
	bool readLayerEnemies(char* start, char* end, LevelData& data) const;
	bool readLevelExit(char* start, char* end, LevelData& data) const;

	// \brief check level bean for validity before loading the level
	bool checkData(LevelData& data) const;

	// \brief update data to prepare it for the level
	void updateData(LevelData& data) const;

	std::map<int, std::string> m_levelItemMap;
	std::map<int, EnemyID> m_enemyMap;

	// this is the width in tiles of the dynamic tile tileset
	// and used to calculate the skin of the dynamic tile.
	const int DYNAMIC_TILE_COUNT = 20;

	const char* LEVEL_NAME = "level.name";
	const char* MAP_SIZE = "map.size";
	const char* LEVEL_EXIT = "level.exit";
	const char* MAP_TILESIZE = "map.tilesize";
	const char* TILESET_PATH = "tileset.path";
	const char* LAYER_COLLIDABLE = "layer.collidable";
	const char* LAYER_TILES_BACKGROUND = "layer.tiles.background";
	const char* LAYER_TILES_FOREGROUND = "layer.tiles.foreground";
	const char* LAYER_DYNAMIC_TILES = "layer.dynamictiles";
	const char* LAYER_LEVEL_ITEMS = "layer.levelitems";
	const char* LAYER_ENEMIES = "layer.enemies";
	const char* LAYER_BACKGROUND = "layer.background";
};