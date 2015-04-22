#pragma once

#include "global.h"
#include "BackgroundLayer.h"
#include "DynamicTile.h"
#include "Logger.h"

struct LevelData
{
	std::string name;
	sf::Vector2i mapSize;
	sf::Vector2i tileSize;
	sf::Vector2f startPos;
	std::string tileSetPath;
	std::vector<BackgroundLayer> backgroundLayers;
	std::vector<std::vector<int>> layers;
	std::vector<bool> collidableTiles;
	std::vector<std::vector<bool>> collidableTileRects;
	std::vector<std::pair<DynamicTileID, std::vector<bool>>> dynamicTiles;
	std::vector<std::pair<DynamicTileID, sf::Vector2f>> dynamicTileRects;
	sf::FloatRect levelRect;
};

class LevelReader
{
public:
	LevelReader();
	~LevelReader();

	bool readLevel(char* fileName, LevelData& data);

private:

	char* gotoNextChar(char* buffer, char* end, char goal);
	int countToNextChar(char* buffer, char* end, char goal);

	bool readLevelName(char* start, char* end, LevelData& data);
	bool readTilesetPath(char* start, char* end, LevelData& data);
	bool readMapSize(char* start, char* end, LevelData& data);
	bool readTileSize(char* start, char* end, LevelData& data);
	bool readLayerBackground(char* start, char* end, LevelData& data);
	bool readLayerCollidable(char* start, char* end, LevelData& data);
	bool readStartPos(char* start, char* end, LevelData& data);
	bool readLayerTiles(char* start, char* end, LevelData& data);
	bool readLayerDynamicTiles(char* start, char* end, LevelData& data);

	// \brief check level bean for validity before loading the level
	bool checkData(LevelData& data);

	// \brief update data to prepare it for the level
	void updateData(LevelData& data);
	DynamicTileID resolveDynamicTile(int tileID);

	const char COMMENT_MARKER = '#';
	const char* LEVEL_NAME = "level.name";
	const char* MAP_SIZE = "map.size";
	const char* MAP_TILESIZE = "map.tilesize";
	const char* TILESET_PATH = "tileset.path";
	const char* LAYER_COLLIDABLE = "layer.collidable";
	const char* LAYER_TILES = "layer.tiles";
	const char* LAYER_DYNAMIC_TILES = "layer.dynamictiles";
	const char* LAYER_BACKGROUND = "layer.background";
	const char* CENDRIC_STARTPOS = "cendric.startpos";
};