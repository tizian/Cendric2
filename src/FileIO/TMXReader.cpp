#include "FileIO/TMXReader.h"

#ifndef XMLCheckResult
#define XMLCheckResult(result) if (result != tinyxml2::XML_SUCCESS) {g_logger->logError("MapReader", "XML file could not be read, error: " + std::to_string(static_cast<int>(result))); return false; }
#endif

using namespace std;

void TMXReader::logError(const std::string& error) const {
	g_logger->logError("TMXReader", "Error in tmx data : " + error);
}

bool TMXReader::checkData(TMXData& data) const {
	if (data.mapSize.x == 0 || data.mapSize.y == 0) {
		logError("map size not set / invalid");
		return false;
	}
	if (data.tileSize.x == 0 || data.tileSize.y == 0) {
		logError("tile size not set / invalid");
		return false;
	}
	if (data.name.empty()) {
		logError("map name not set / empty");
		return false;
	}
	if (data.tileSetPath.empty()) {
		logError("tileset path not set / empty");
		return false;
	}
	for (int i = 0; i < static_cast<int>(data.backgroundTileLayers.size()); i++) {
		if (data.backgroundTileLayers[i].empty()) {
			logError("background layer " + std::to_string(i) + std::string(" empty"));
			return false;
		}
		if (data.backgroundTileLayers[i].size() != data.mapSize.x * data.mapSize.y) {
			logError("background layer " + std::to_string(i) + std::string(" has not correct size (map size)"));
			return false;
		}
	}
	for (int i = 0; i < data.foregroundTileLayers.size(); i++) {
		if (data.foregroundTileLayers[i].empty()) {
			logError("foreground layer " + std::to_string(i) + std::string(" empty"));
			return false;
		}
		if (static_cast<int>(data.foregroundTileLayers[i].size()) != data.mapSize.x * data.mapSize.y) {
			logError("foreground layer " + std::to_string(i) + std::string(" has not correct size (map size)"));
			return false;
		}
	}
	for (int i = 0; i < static_cast<int>(data.lightedForegroundTileLayers.size()); i++) {
		if (data.lightedForegroundTileLayers[i].empty()) {
			logError("lighted foreground layer " + std::to_string(i) + std::string(" empty"));
			return false;
		}
		if (static_cast<int>(data.lightedForegroundTileLayers[i].size()) != data.mapSize.x * data.mapSize.y) {
			logError("lighted foreground layer " + std::to_string(i) + std::string(" has not correct size (map size)"));
			return false;
		}
	}
	if (data.collidableTiles.empty()) {
		logError("collidable layer is empty");
		return false;
	}
	if (static_cast<int>(data.collidableTiles.size()) != data.mapSize.x * data.mapSize.y) {
		logError("collidable layer has not correct size (map size)");
		return false;
	}

	return true;
}

bool TMXReader::readLights(tinyxml2::XMLElement* objectgroup, TMXData& data) const {
	tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");

	while (object != nullptr) {
		int x;
		tinyxml2::XMLError result = object->QueryIntAttribute("x", &x);
		XMLCheckResult(result);

		int y;
		result = object->QueryIntAttribute("y", &y);
		XMLCheckResult(result);

		int width;
		result = object->QueryIntAttribute("width", &width);
		XMLCheckResult(result);

		int height;
		result = object->QueryIntAttribute("height", &height);
		XMLCheckResult(result);

		LightData lightData;
		lightData.radius.x = width / 2.f;
		lightData.radius.y = height / 2.f;
		lightData.center.x = x + lightData.radius.x;
		lightData.center.y = y + lightData.radius.y;

		// brightness for light bean
		tinyxml2::XMLElement* properties = object->FirstChildElement("properties");
		if (properties != nullptr) {
			tinyxml2::XMLElement* _property = properties->FirstChildElement("property");
			while (_property != nullptr) {
				const char* textAttr = nullptr;
				textAttr = _property->Attribute("name");
				if (textAttr == nullptr) {
					logError("XML file could not be read, no objectgroup->object->properties->property->name attribute found for light bean.");
					return false;
				}
				std::string name = textAttr;

				if (name.compare("brightness") == 0) {
					float brightness;
					result = _property->QueryFloatAttribute("value", &brightness);
					XMLCheckResult(result);
					if (brightness < 0.f || brightness > 1.f) {
						brightness = 1.f;
						g_logger->logWarning("TMXReader", "Brightness must be between 0 and 1. It was " + std::to_string(brightness) + ", it is now 1");
					}
					lightData.brightness = brightness;
				}
				else {
					logError("XML file could not be read, unknown objectgroup->object->properties->property->name attribute found for light bean.");
					return false;
				}
				_property = _property->NextSiblingElement("property");
			}
		}

		data.lights.push_back(lightData);
		object = object->NextSiblingElement("object");
	}
	return true;
}

bool TMXReader::readBackgroundTileLayer(const std::string& layer, TMXData& data) const {
	std::string layerData = layer;

	size_t pos = 0;
	std::vector<int> backgroundLayer;
	while ((pos = layerData.find(",")) != std::string::npos) {
		backgroundLayer.push_back(std::stoi(layerData.substr(0, pos)));
		layerData.erase(0, pos + 1);
	}
	backgroundLayer.push_back(std::stoi(layerData));

	data.backgroundTileLayers.push_back(backgroundLayer);
	return true;
}

bool TMXReader::readCollidableLayer(const std::string& layer, TMXData& data) const {
	std::string layerData = layer;

	size_t pos = 0;
	std::vector<bool> collidableLayer;
	while ((pos = layerData.find(",")) != std::string::npos) {
		collidableLayer.push_back(std::stoi(layerData.substr(0, pos)) != 0);
		layerData.erase(0, pos + 1);
	}
	collidableLayer.push_back(std::stoi(layerData) != 0);

	data.collidableTiles = collidableLayer;
	return true;
}

bool TMXReader::readForegroundTileLayer(const std::string& layer, TMXData& data) const {
	std::string layerData = layer;

	size_t pos = 0;
	std::vector<int> foregroundLayer;
	while ((pos = layerData.find(",")) != std::string::npos) {
		foregroundLayer.push_back(std::stoi(layerData.substr(0, pos)));
		layerData.erase(0, pos + 1);
	}
	foregroundLayer.push_back(std::stoi(layerData));

	data.foregroundTileLayers.push_back(foregroundLayer);
	return true;
}

bool TMXReader::readLightedForegroundTileLayer(const std::string& layer, TMXData& data) const {
	std::string layerData = layer;

	size_t pos = 0;
	std::vector<int> foregroundLayer;
	while ((pos = layerData.find(",")) != std::string::npos) {
		foregroundLayer.push_back(std::stoi(layerData.substr(0, pos)));
		layerData.erase(0, pos + 1);
	}
	foregroundLayer.push_back(std::stoi(layerData));

	data.lightedForegroundTileLayers.push_back(foregroundLayer);
	return true;
}

bool TMXReader::readMapName(tinyxml2::XMLElement* _property, TMXData& data) const {
	// we've found the property "name"
	const char* textAttr = nullptr;
	textAttr = _property->Attribute("value");
	if (textAttr == nullptr) {
		logError("XML file could not be read, no value attribute found (map->properties->property->name=name).");
		return false;
	}
	data.name = textAttr;
	return true;
}

bool TMXReader::readTilesetPath(tinyxml2::XMLElement* _property, TMXData& data) const {
	// we've found the property "tilesetpath"
	const char* textAttr = nullptr;
	textAttr = _property->Attribute("value");
	if (textAttr == nullptr) {
		logError("XML file could not be read, no value attribute found (map->properties->property->name=tilesetpath).");
		return false;
	}
	data.tileSetPath = textAttr;
	return true;
}

bool TMXReader::readMusicPath(tinyxml2::XMLElement* _property, TMXData& data) const {
	// we've found the property "musicpath"
	const char* textAttr = nullptr;
	textAttr = _property->Attribute("value");
	if (textAttr == nullptr) {
		logError("XML file could not be read, no value attribute found (map->properties->property->name=musicpath).");
		return false;
	}
	data.musicPath = textAttr;
	return true;
}

bool TMXReader::readDimming(tinyxml2::XMLElement* _property, TMXData& data) const {
	// we've found the property "dimming"
	float dimming = 0.f;
	tinyxml2::XMLError result = _property->QueryFloatAttribute("value", &dimming);
	XMLCheckResult(result);

	if (dimming < 0.0f || dimming > 1.0f) {
		logError("XML file could not be read, dimming value not allowed (only [0,1]).");
		return false;
	}

	data.dimming = dimming;
	return true;
}

bool TMXReader::readBackgroundLayers(tinyxml2::XMLElement* _property, TMXData& data) const {
	// nop, is used by the level only and has no effect for the map.
	return true;
}

bool TMXReader::readAnimatedTiles(tinyxml2::XMLElement* map, TMXData& data) const {
	tinyxml2::XMLElement* tileset = map->FirstChildElement("tileset");
	while (tileset != nullptr) {
		int firstGid;
		tinyxml2::XMLError result = tileset->QueryIntAttribute("firstgid", &firstGid);
		XMLCheckResult(result);
		tinyxml2::XMLElement* tile = tileset->FirstChildElement("tile");
		while (tile != nullptr) {
			AnimatedTileData tileData;
			result = tile->QueryIntAttribute("id", &tileData.tileID);
			XMLCheckResult(result);
			tileData.tileID += firstGid;
			tinyxml2::XMLElement* animation = tile->FirstChildElement("animation");
			if (animation != nullptr) {
				tinyxml2::XMLElement* frame = animation->FirstChildElement("frame");
				while (frame != nullptr) {
					std::pair<int, sf::Time> frameData;
					result = frame->QueryIntAttribute("tileid", &frameData.first);
					XMLCheckResult(result);
					frameData.first += firstGid;
					int milliseconds;
					result = frame->QueryIntAttribute("duration", &milliseconds);
					XMLCheckResult(result);
					frameData.second = sf::milliseconds(milliseconds);
					tileData.frames.push_back(frameData);
					frame = frame->NextSiblingElement("frame");
				}
			}
			if (!tileData.frames.empty()) {
				data.animatedTiles.push_back(tileData);
			}
			tile = tile->NextSiblingElement("tile");
		}
		tileset = tileset->NextSiblingElement("tileset");
	}
	return true;
}

bool TMXReader::readMapProperties(tinyxml2::XMLElement* map, TMXData& data) const {
	// check if renderorder is correct
	const char* textAttr = nullptr;
	textAttr = map->Attribute("renderorder");
	if (textAttr == nullptr) {
		logError("XML file could not be read, no renderorder attribute found.");
		return false;
	}
	std::string renderorder = textAttr;
	if (renderorder.compare("right-down") != 0) {
		logError("XML file could not be read, renderorder is not \"right-down\".");
		return false;
	}

	// check if orientation is correct
	textAttr = nullptr;
	textAttr = map->Attribute("orientation");
	if (textAttr == nullptr) {
		logError("XML file could not be read, no orientation attribute found.");
		return false;
	}
	std::string orientation = textAttr;
	if (orientation.compare("orthogonal") != 0) {
		logError("XML file could not be read, renderorder is not \"orthogonal\".");
		return false;
	}

	// read map width and height
	tinyxml2::XMLError result = map->QueryIntAttribute("width", &data.mapSize.x);
	XMLCheckResult(result);
	result = map->QueryIntAttribute("height", &data.mapSize.y);
	XMLCheckResult(result);

	// read tile size
	result = map->QueryIntAttribute("tilewidth", &data.tileSize.x);
	XMLCheckResult(result);
	result = map->QueryIntAttribute("tileheight", &data.tileSize.y);
	XMLCheckResult(result);

	// read level properties
	tinyxml2::XMLElement* properties = map->FirstChildElement("properties");
	if (properties == nullptr) {
		logError("XML file could not be read, no properties node found.");
		return false;
	}
	tinyxml2::XMLElement* _property = properties->FirstChildElement("property");

	while (_property != nullptr) {
		textAttr = nullptr;
		textAttr = _property->Attribute("name");
		if (textAttr == nullptr) {
			logError("XML file could not be read, no property->name attribute found.");
			return false;
		}
		std::string name = textAttr;
		if (name.compare("name") == 0) {
			if (!readMapName(_property, data)) return false;
		}
		else if (name.compare("backgroundlayers") == 0) {
			if (!readBackgroundLayers(_property, data)) return false;
		}
		else if (name.compare("tilesetpath") == 0) {
			if (!readTilesetPath(_property, data)) return false;
		}
		else if (name.compare("musicpath") == 0) {
			if (!readMusicPath(_property, data)) return false;
		}
		else if (name.compare("dimming") == 0) {
			if (!readDimming(_property, data)) return false;
		}
		else {
			logError("XML file could not be read, unknown name attribute found in properties (map->properties->property->name).");
			return false;
		}

		_property = _property->NextSiblingElement("property");
	}

	return true;
}

void TMXReader::updateData(TMXData& data) const {
	int x = 0;
	int y = 0;

	vector<bool> xLine;

	// calculate collidable tiles
	for (std::vector<bool>::iterator it = data.collidableTiles.begin(); it != data.collidableTiles.end(); ++it) {

		xLine.push_back((*it));
		if (x + 1 >= data.mapSize.x) {
			x = 0;
			y++;
			data.collidableTilePositions.push_back(xLine); // push back creates a copy of that vector.
			xLine.clear();
		}
		else {
			x++;
		}
	}

	int tileWidth = data.tileSize.x;
	int tileHeight = data.tileSize.y;

	// calculate map rect
	data.mapRect.left = 0;
	data.mapRect.top = 0;
	data.mapRect.height = static_cast<float>(data.tileSize.y * data.mapSize.y);
	data.mapRect.width = static_cast<float>(data.tileSize.x * data.mapSize.x);
}