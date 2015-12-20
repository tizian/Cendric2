#pragma once

#include "global.h"
#include "Logger.h"
#include "tinyxml2/tinyxml2.h"
#include "Structs/TMXData.h"

class TMXReader {
public:
	virtual ~TMXReader() {};

protected:
	virtual void logError(const std::string& error) const;

	// reads properties name, tile size, map size, tileset, dimming starting @map node
	bool readMapProperties(tinyxml2::XMLElement* map, TMXData& data) const;
	bool readMapName(tinyxml2::XMLElement* _property, TMXData& data) const;
	bool readTilesetPath(tinyxml2::XMLElement* _property, TMXData& data) const;
	bool readMusicPath(tinyxml2::XMLElement* _property, TMXData& data) const;
	bool readDimming(tinyxml2::XMLElement* _property, TMXData& data) const;
	bool readAnimatedTiles(tinyxml2::XMLElement* map, TMXData& data) const;
	virtual bool readBackgroundLayers(tinyxml2::XMLElement* _property, TMXData& data) const;

	bool readBackgroundTileLayer(const std::string& layer, TMXData& data) const;
	bool readLightedForegroundTileLayer(const std::string& layer, TMXData& data) const;
	bool readForegroundTileLayer(const std::string& layer, TMXData& data) const;
	bool readCollidableLayer(const std::string& layer, TMXData& data) const;
	bool readLights(tinyxml2::XMLElement* objects, TMXData& data) const;

	// check map bean for validity before loading the map
	bool checkData(TMXData& data) const;

	// update data to prepare it for the map
	void updateData(TMXData& data) const;
};