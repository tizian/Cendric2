#pragma once

#include "global.h"
#include "Logger.h"
#include "tinyxml2/tinyxml2.h"

#include "Structs/MapData.h"
#include "Enums/MapDynamicTileID.h"

class MapReader {
public:
	MapReader();
	~MapReader();

	bool readMap(const char* fileName, MapData& data);

private:
	// reads properties name, tile size, map size, tileset, dimming starting @map node
	bool readMapProperties(tinyxml2::XMLElement* map, MapData& data) const;
	bool readMapName(tinyxml2::XMLElement* _property, MapData& data) const;
	bool readTilesetPath(tinyxml2::XMLElement* _property, MapData& data) const;
	bool readMusicPath(tinyxml2::XMLElement* _property, MapData& data) const;
	bool readDimming(tinyxml2::XMLElement* _property, MapData& data) const;
	bool readFirstGridIDs(tinyxml2::XMLElement* map, MapData& data);

	bool readLayers(tinyxml2::XMLElement* map, MapData& data) const;
	bool readBackgroundTileLayer(const std::string& layer, MapData& data) const;
	bool readLightedForegroundTileLayer(const std::string& layer, MapData& data) const;
	bool readForegroundTileLayer(const std::string& layer, MapData& data) const;
	bool readCollidableLayer(const std::string& layer, MapData& data) const;
	bool readDynamicTileLayer(MapDynamicTileID id, const std::string& layer, MapData& data) const;

	bool readObjects(tinyxml2::XMLElement* map, MapData& data) const;
	bool readMapExits(tinyxml2::XMLElement* objects, MapData& data) const;
	bool readNPCs(tinyxml2::XMLElement* objects, MapData& data) const;
	bool readLights(tinyxml2::XMLElement* objects, MapData& data) const;

	// check map bean for validity before loading the map
	bool checkData(MapData& data) const;

	// update data to prepare it for the map
	void updateData(MapData& data) const;

	// this is the width in tiles of the dynamic tile tileset
	// and used to calculate the skin of the dynamic tile.
	const int DYNAMIC_TILE_COUNT = 20;

	int m_firstGidDynamicTiles;
};