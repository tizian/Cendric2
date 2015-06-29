#pragma once

#include "global.h"
#include "FileIO/Reader.h"
#include "Logger.h"

#include "Structs/MapData.h"
#include "Enums/NpcID.h"

class MapReader : public Reader
{
public:
	MapReader();
	~MapReader();
	bool readMap(char* fileName, MapData& data);

private:
	void initMaps();

	std::map<int, NpcID> m_npcMap;

	bool readMapName(char* start, char* end, MapData& data) const;
	bool readTilesetPath(char* start, char* end, MapData& data) const;
	bool readMapSize(char* start, char* end, MapData& data) const;
	bool readTileSize(char* start, char* end, MapData& data) const;
	bool readLayerCollidable(char* start, char* end, MapData& data) const;
	bool readBackgroundLayerTiles(char* start, char* end, MapData& data) const;
	bool readForegroundLayerTiles(char* start, char* end, MapData& data) const;
	bool readLevelEntry(char* start, char* end, MapData& data) const;
	bool readLayerNPC(char* start, char* end, MapData& data) const;

	// \brief check map bean for validity before loading the level
	bool checkData(MapData& data) const;

	// \brief update data to prepare it for the map
	void updateData(MapData& data) const;

	const char* MAP_NAME = "map.name";
	const char* MAP_SIZE = "map.size";
	const char* MAP_TILESIZE = "map.tilesize";
	const char* TILESET_PATH = "tileset.path";
	const char* LEVEL_ENTRY = "level.entry";
	const char* LAYER_COLLIDABLE = "layer.collidable";
	const char* LAYER_TILES_BACKGROUND = "layer.tiles.background";
	const char* LAYER_TILES_FOREGROUND = "layer.tiles.foreground";
	const char* LAYER_NPC = "layer.npc";
};