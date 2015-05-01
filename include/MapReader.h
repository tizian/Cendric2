#pragma once

#include "global.h"
#include "Logger.h"

class Map;

struct MapData
{
	std::string name;
	sf::Vector2i mapSize;
	sf::Vector2i tileSize;
	sf::Vector2f startPos;
	std::string tileSetPath;
	std::vector<std::vector<int>> layers;
	std::vector<bool> collidableTiles;
	std::vector<std::vector<bool>> collidableTileRects;
	sf::FloatRect mapRect;
};

class MapReader
{
public:
	MapReader();
	~MapReader();

	bool readMap(char* fileName, MapData& data);

private:

	char* gotoNextChar(char* buffer, char* end, char goal) const;
	int countToNextChar(char* buffer, char* end, char goal) const;

	bool readMapName(char* start, char* end, MapData& data) const;
	bool readTilesetPath(char* start, char* end, MapData& data) const;
	bool readMapSize(char* start, char* end, MapData& data) const;
	bool readTileSize(char* start, char* end, MapData& data) const;
	bool readLayerCollidable(char* start, char* end, MapData& data) const;
	bool readStartPos(char* start, char* end, MapData& data) const;
	bool readLayerTiles(char* start, char* end, MapData& data) const;

	// \brief check map bean for validity before loading the level
	bool checkData(MapData& data) const;

	// \brief update data to prepare it for the map
	void updateData(MapData& data) const;

	const char COMMENT_MARKER = '#';
	const char* MAP_NAME = "map.name";
	const char* MAP_SIZE = "map.size";
	const char* MAP_TILESIZE = "map.tilesize";
	const char* TILESET_PATH = "tileset.path";
	const char* LAYER_COLLIDABLE = "layer.collidable";
	const char* LAYER_TILES = "layer.tiles";
	const char* CENDRIC_STARTPOS = "cendric.startpos";
};