#pragma once

#include "global.h"
#include "Reader.h"
#include "Logger.h"

#include "Structs/MapData.h"

class MapReader : public Reader
{
public:

	bool readMap(char* fileName, MapData& data);

private:

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

	const char* MAP_NAME = "map.name";
	const char* MAP_SIZE = "map.size";
	const char* MAP_TILESIZE = "map.tilesize";
	const char* TILESET_PATH = "tileset.path";
	const char* LAYER_COLLIDABLE = "layer.collidable";
	const char* LAYER_TILES = "layer.tiles";
	const char* CENDRIC_STARTPOS = "cendric.startpos";
};