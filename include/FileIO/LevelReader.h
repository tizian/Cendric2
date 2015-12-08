#pragma once

#include "global.h"
#include "Logger.h"
#include "tinyxml2/tinyxml2.h"

#include "BackgroundLayer.h"
#include "Level/LevelDynamicTile.h"
#include "Level/LevelItem.h"
#include "Level/Enemy.h"

#include "Structs/LevelData.h"
#include "Enums/LevelDynamicTileID.h"

class LevelReader {
public:
	LevelReader();
	~LevelReader();

	bool readLevel(const std::string& fileName, LevelData& data);

private:
	void initMaps();

	// reads properties name, tile size, map size, background layers, tileset, dimming starting @map node
	bool readLevelProperties(tinyxml2::XMLElement* map, LevelData& data) const;
	bool readLevelName(tinyxml2::XMLElement* _property, LevelData& data) const;
	bool readBackgroundLayers(tinyxml2::XMLElement* _property, LevelData& data) const;
	bool readTilesetPath(tinyxml2::XMLElement* _property, LevelData& data) const;
	bool readMusicPath(tinyxml2::XMLElement* _property, LevelData& data) const;
	bool readDimming(tinyxml2::XMLElement* _property, LevelData& data) const;

	bool readFirstGridIDs(tinyxml2::XMLElement* map, LevelData& data);
	bool readItemIDs(tinyxml2::XMLElement* firstTile);

	bool readLayers(tinyxml2::XMLElement* map, LevelData& data) const;
	bool readBackgroundTileLayer(const std::string& layer, LevelData& data) const;
	bool readLightedForegroundTileLayer(const std::string& layer, LevelData& data) const;
	bool readForegroundTileLayer(const std::string& layer, LevelData& data) const;
	bool readDynamicTileLayer(LevelDynamicTileID id, const std::string& layer, LevelData& data) const;
	bool readLeverLayer(const std::string& layer, LevelData& data) const;
	bool readLevelItemLayer(const std::string& layer, LevelData& data) const;
	bool readCollidableLayer(const std::string& layer, LevelData& data) const;

	bool readObjects(tinyxml2::XMLElement* map, LevelData& data) const;
	bool readChestTiles(tinyxml2::XMLElement* objects, LevelData& data) const;
	bool readEnemies(tinyxml2::XMLElement* objects, LevelData& data) const;
	bool readLevelExits(tinyxml2::XMLElement* objects, LevelData & data) const;
	bool readLights(tinyxml2::XMLElement* objects, LevelData & data) const;

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