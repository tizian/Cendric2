#pragma once

#include "global.h"
#include "Logger.h"
#include "tinyxml2/tinyxml2.h"
#include "Structs/WorldData.h"

class CharacterCore;

class WorldReader {
public:
	virtual ~WorldReader() {};

protected:
	virtual void logError(const std::string& error) const;

	// reads properties name, tile size, map size, tileset, dimming starting @map node
	virtual bool readMapProperties(tinyxml2::XMLElement* map, WorldData& data) const;
	bool readTilesetPath(tinyxml2::XMLElement* _property, WorldData& data) const;
	bool readMusicPath(tinyxml2::XMLElement* _property, WorldData& data) const;
	bool readDimming(tinyxml2::XMLElement* _property, WorldData& data) const;
	bool readWeather(tinyxml2::XMLElement* _property, WorldData& data) const;
	bool readTileProperties(tinyxml2::XMLElement* map, WorldData& data);
	virtual bool readBackgroundLayers(tinyxml2::XMLElement* _property, WorldData& data) const;

	bool readBackgroundTileLayer(const std::string& layer, WorldData& data) const;
	bool readLightedForegroundTileLayer(const std::string& layer, WorldData& data) const;
	bool readForegroundTileLayer(const std::string& layer, WorldData& data) const;
	bool readCollidableLayer(const std::string& layer, WorldData& data) const;
	bool readLights(tinyxml2::XMLElement* objects, WorldData& data) const;
	bool readTriggers(tinyxml2::XMLElement* objects, WorldData& data) const;
	virtual bool readSigns(tinyxml2::XMLElement* objects, WorldData& data) const;
	virtual bool readChests(tinyxml2::XMLElement* objects, WorldData& data) const;

	// check map bean for validity before loading the map
	bool checkData(WorldData& data) const;

	// update data to prepare it for the map
	void updateData(WorldData& data) const;

	// returns true if all in the layer properties given layer conditions are fulfilled for that layer
	bool layerConditionsFulfilled(tinyxml2::XMLElement* layer) const;

protected:
	const CharacterCore* m_core;
	std::map<int, LightData> m_lightTiles;

	// used to resolve a light string (x offset, y offset, width, height, brightness)
	// and write into the light data. Returns whether it was successful.
	bool resolveLightString(const std::string& lightString, LightData& data) const;

private:
	void readLightsFromLayers(WorldData& data, std::vector<std::vector<int>>& layers) const;
};