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
#include "FileIO/WorldReader.h"

class LevelReader : public WorldReader {
public:
	LevelReader();
	~LevelReader();

	bool readLevel(const std::string& fileName, LevelData& data, const CharacterCore* core);

private:
	void logError(const std::string& error) const override;

	bool readMapProperties(tinyxml2::XMLElement* map, WorldData& data) const override;
	bool readBackgroundLayers(tinyxml2::XMLElement* _property, WorldData& data) const override;
	bool readFirstGridIDs(tinyxml2::XMLElement* map, LevelData& data);
	bool readItemIDs(tinyxml2::XMLElement* firstTile);

	bool readLayers(tinyxml2::XMLElement* map, LevelData& data) const;
	bool readDynamicTileLayer(const std::string& layer, LevelData& data) const;
	bool readFluidLayer(const std::string& layer, LevelData& data) const;
	bool readLeverLayer(const std::string& layer, LevelData& data) const;
	bool readLevelItemLayer(const std::string& layer, LevelData& data) const;
	
	bool readObjects(tinyxml2::XMLElement* map, LevelData& data) const;
	bool readDoorTiles(tinyxml2::XMLElement* objects, LevelData& data) const;
	bool readChestTiles(tinyxml2::XMLElement* objects, LevelData& data) const;
	bool readModifierTiles(tinyxml2::XMLElement* objects, LevelData& data) const;
	bool readMovingTiles(tinyxml2::XMLElement* objects, LevelData& data) const;
	bool readJumpingTiles(tinyxml2::XMLElement* objects, LevelData& data) const;
	bool readSignTiles(tinyxml2::XMLElement* objects, LevelData& data) const;
	bool readLadderTiles(tinyxml2::XMLElement* objects, LevelData& data) const;
	bool readEnemies(tinyxml2::XMLElement* objects, LevelData& data) const;

	// this is the width in tiles of the dynamic tile tileset
	// and used to calculate the skin of the dynamic tile.
	const int DYNAMIC_TILE_COUNT = 30;

	// \brief check level bean for validity before loading the level
	bool checkData(LevelData& data) const;

	std::map<int, std::string> m_levelItemMap;

	int m_firstGidEnemies;
	int m_firstGidItems;
	int m_firstGidDynamicTiles;
};