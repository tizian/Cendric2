#pragma once

#include "global.h"
#include "Logger.h"
#include "tinyxml2/tinyxml2.h"

#include "BackgroundLayer.h"
#include "DynamicTile.h"
#include "LevelItem.h"
#include "Enemy.h"

#include "Structs/LevelData.h"
#include "Enums/DynamicTileID.h"

using namespace tinyxml2;

class LevelReader {
public:
	LevelReader();
	~LevelReader();

	bool readLevel(const std::string& fileName, LevelData& data);

private:
	void initMaps();

	// reads properties name, tile size, map size, background layers, tileset, dimming starting @map node
	bool readLevelProperties(XMLElement* map, LevelData& data) const;
	bool readLevelName(XMLElement* _property, LevelData& data) const;
	bool readBackgroundLayers(XMLElement* _property, LevelData& data) const;
	bool readTilesetPath(XMLElement* _property, LevelData& data) const;
	bool readMusicPath(XMLElement* _property, LevelData& data) const;
	bool readDimming(XMLElement* _property, LevelData& data) const;

	bool readFirstGridIDs(XMLElement* map, LevelData& data);
	bool readItemIDs(XMLElement* firstTile);

	bool readLayers(XMLElement* map, LevelData& data) const;
	bool readBackgroundTileLayer(const std::string& layer, LevelData& data) const;
	bool readForegroundTileLayer(const std::string& layer, LevelData& data) const;
	bool readDynamicTileLayer(DynamicTileID id, const std::string& layer, LevelData& data) const;
	bool readLevelItemLayer(const std::string& layer, LevelData& data) const;
	bool readCollidableLayer(const std::string& layer, LevelData& data) const;

	bool readObjects(XMLElement* map, LevelData& data) const;
	bool readChestTiles(XMLElement* objects, LevelData& data) const;
	bool readEnemies(XMLElement* objects, LevelData& data) const;
	bool readLevelExits(XMLElement* objects, LevelData & data) const;
	bool readLights(XMLElement* objects, LevelData & data) const;

	// this is the width in tiles of the dynamic tile tileset
	// and used to calculate the skin of the dynamic tile.
	const int DYNAMIC_TILE_COUNT = 20;

	// \brief check level bean for validity before loading the level
	bool checkData(LevelData& data) const;

	// \brief update data to prepare it for the level
	void updateData(LevelData& data) const;

	std::map<int, std::string> m_levelItemMap;
	std::map<int, EnemyID> m_enemyMap;

	int m_firstGidEnemies;
	int m_firstGidItems;
	int m_firstGidDynamicTiles;
};