#include "FileIO/WorldReader.h"
#include "CharacterCore.h"

#ifndef XMLCheckResult
#define XMLCheckResult(result) if (result != tinyxml2::XML_SUCCESS) {g_logger->logError("MapReader", "XML file could not be read, error: " + std::to_string(static_cast<int>(result))); return false; }
#endif

using namespace std;

void WorldReader::logError(const std::string& error) const {
	g_logger->logError("WorldReader", "Error in world data : " + error);
}

bool WorldReader::checkData(WorldData& data) const {
	if (data.mapSize.x == 0 || data.mapSize.y == 0) {
		logError("map size not set / invalid");
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
	for (size_t i = 0; i < data.backgroundTileLayers.size(); ++i) {
		if (data.backgroundTileLayers[i].empty()) {
			logError("background layer " + std::to_string(i) + std::string(" empty"));
			return false;
		}
		if (static_cast<int>(data.backgroundTileLayers[i].size()) != data.mapSize.x * data.mapSize.y) {
			logError("background layer " + std::to_string(i) + std::string(" has not correct size (map size)"));
			return false;
		}
	}
	for (size_t i = 0; i < data.foregroundTileLayers.size(); ++i) {
		if (data.foregroundTileLayers[i].empty()) {
			logError("foreground layer " + std::to_string(i) + std::string(" empty"));
			return false;
		}
		if (static_cast<int>(data.foregroundTileLayers[i].size()) != data.mapSize.x * data.mapSize.y) {
			logError("foreground layer " + std::to_string(i) + std::string(" has not correct size (map size)"));
			return false;
		}
	}
	for (size_t i = 0; i < data.lightedForegroundTileLayers.size(); ++i) {
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

bool WorldReader::readLights(tinyxml2::XMLElement* objectgroup, WorldData& data) const {
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
						g_logger->logWarning("WorldReader", "Brightness must be between 0 and 1. It was " + std::to_string(brightness) + ", it is now 1");
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

bool WorldReader::readTriggers(tinyxml2::XMLElement* objectgroup, WorldData& data) const {
	tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");

	while (object != nullptr) {
		TriggerData trigger;
		trigger.worldID = data.id;
		tinyxml2::XMLError result = object->QueryIntAttribute("id", &trigger.objectID);
		XMLCheckResult(result);

		int x;
		result = object->QueryIntAttribute("x", &x);
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

		trigger.triggerRect.left = static_cast<float>(x);
		trigger.triggerRect.top = static_cast<float>(y);
		trigger.triggerRect.width = static_cast<float>(width);
		trigger.triggerRect.height = static_cast<float>(height);

		// trigger properties
		tinyxml2::XMLElement* properties = object->FirstChildElement("properties");
		if (properties != nullptr) {
			tinyxml2::XMLElement* _property = properties->FirstChildElement("property");
			while (_property != nullptr) {
				const char* textAttr = nullptr;
				textAttr = _property->Attribute("name");
				if (textAttr == nullptr) {
					logError("XML file could not be read, no property->name attribute found for trigger.");
					return false;
				}
				std::string name = textAttr;

				if (name.compare("hint") == 0) {
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, hint value property not found.");
						return false;
					}
					TriggerContent content(TriggerContentType::Hint);
					content.s1 = textAttr;
					trigger.content.push_back(content);
				}
				else if (name.compare("condition progress") == 0) {
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, condition progress value property not found.");
						return false;
					}

					std::string conditionProgress = textAttr;

					size_t pos = 0;
					if ((pos = conditionProgress.find(",")) == std::string::npos) {
						logError("XML file could not be read, condition progress value must be two strings, seperated by a comma.");
						return false;
					}

					TriggerContent content(TriggerContentType::ConditionProgress);
					content.s1 = conditionProgress.substr(0, pos);
					conditionProgress.erase(0, pos + 1);
					content.s2 = conditionProgress;
					trigger.content.push_back(content);
				}
				else if (name.compare("condition") == 0) {
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, condition value property not found.");
						return false;
					}

					trigger.condition = textAttr;
				}
				else if (name.compare("map entry") == 0) {
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, map entry value property not found.");
						return false;
					}

					std::string mapEntry = textAttr;

					size_t pos = 0;
					if ((pos = mapEntry.find(",")) == std::string::npos) {
						logError("XML file could not be read, map entry value must be a string (map id) and the x and y coords, seperated by commas.");
						return false;
					}

					TriggerContent content(TriggerContentType::MapEntry);
					content.s1 = mapEntry.substr(0, pos);
					mapEntry.erase(0, pos + 1);

					if ((pos = mapEntry.find(",")) == std::string::npos) {
						logError("XML file could not be read, map entry value must be a string (map id) and the x and y coords, seperated by commas.");
						return false;
					}
					content.i1 = std::atoi(mapEntry.substr(0, pos).c_str());
					mapEntry.erase(0, pos + 1);

					content.i2 = std::atoi(mapEntry.c_str());
					trigger.content.push_back(content);
				}
				else if (name.compare("level entry") == 0) {
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, level entry value property not found.");
						return false;
					}

					std::string mapEntry = textAttr;

					size_t pos = 0;
					if ((pos = mapEntry.find(",")) == std::string::npos) {
						logError("XML file could not be read, level entry value must be a string (level id) and the x and y coords, seperated by commas.");
						return false;
					}

					TriggerContent content(TriggerContentType::LevelEntry);
					content.s1 = mapEntry.substr(0, pos);
					mapEntry.erase(0, pos + 1);

					if ((pos = mapEntry.find(",")) == std::string::npos) {
						logError("XML file could not be read, level entry value must be a string (level id) and the x and y coords, seperated by commas.");
						return false;
					}
					content.i1 = std::atoi(mapEntry.substr(0, pos).c_str());
					mapEntry.erase(0, pos + 1);

					content.i2 = std::atoi(mapEntry.c_str());
					trigger.content.push_back(content);
				}
				else if (name.compare("persistent") == 0) {
					trigger.isPersistent = true;
				}
				else if (name.compare("keyguarded") == 0) {
					trigger.isKeyGuarded = true;
				}
				else {
					logError("XML file could not be read, unknown property->name attribute found for trigger: " + name);
					return false;
				}
				_property = _property->NextSiblingElement("property");
			}
		}

		data.triggers.push_back(trigger);
		object = object->NextSiblingElement("object");
	}
	return true;
}


bool WorldReader::readBackgroundTileLayer(const std::string& layer, WorldData& data) const {
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

bool WorldReader::readCollidableLayer(const std::string& layer, WorldData& data) const {
	std::string layerData = layer;

	size_t pos = 0;
	size_t index = 0;
	size_t maxindex = data.collidableTiles.size() - 1;
	while ((pos = layerData.find(",")) != std::string::npos && maxindex >= index) {
		if (std::stoi(layerData.substr(0, pos)) != 0) {
			data.collidableTiles.at(index) = true;
		}
		layerData.erase(0, pos + 1);
		index++;
	}
	if (std::stoi(layerData) != 0 && maxindex >= index) {
		data.collidableTiles.at(index) = true;
	}

	return true;
}

bool WorldReader::readForegroundTileLayer(const std::string& layer, WorldData& data) const {
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

bool WorldReader::readLightedForegroundTileLayer(const std::string& layer, WorldData& data) const {
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

bool WorldReader::readMapName(tinyxml2::XMLElement* _property, WorldData& data) const {
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

bool WorldReader::readTilesetPath(tinyxml2::XMLElement* _property, WorldData& data) const {
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

bool WorldReader::readMusicPath(tinyxml2::XMLElement* _property, WorldData& data) const {
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

bool WorldReader::readDimming(tinyxml2::XMLElement* _property, WorldData& data) const {
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

bool WorldReader::readBackgroundLayers(tinyxml2::XMLElement* _property, WorldData& data) const {
	// nop, is used by the level only and has no effect for the map.
	return true;
}

bool WorldReader::readAnimatedTiles(tinyxml2::XMLElement* map, WorldData& data) const {
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

bool WorldReader::readMapProperties(tinyxml2::XMLElement* map, WorldData& data) const {
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

	// pre-load collidable layer
	data.collidableTiles.clear();
	for (int i = 0; i < data.mapSize.x * data.mapSize.y; ++i) {
		data.collidableTiles.push_back(false);
	}

	// read tile size
	sf::Vector2i tileSize;
	result = map->QueryIntAttribute("tilewidth", &tileSize.x);
	XMLCheckResult(result);
	result = map->QueryIntAttribute("tileheight", &tileSize.y);
	XMLCheckResult(result);
	if (tileSize.x != TILE_SIZE || tileSize.y != TILE_SIZE) {
		logError("The tilesize for level and map must be " + std::to_string(TILE_SIZE));
		return false;
	}

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

bool WorldReader::layerConditionsFulfilled(tinyxml2::XMLElement* layer) const {
	tinyxml2::XMLElement* properties = layer->FirstChildElement("properties");
	if (properties == nullptr) {
		return true;
	}

	tinyxml2::XMLElement* _property = properties->FirstChildElement("property");

	while (_property != nullptr) {
		const char* textAttr = nullptr;
		textAttr = _property->Attribute("name");
		if (textAttr == nullptr) {
			logError("XML file could not be read, no property->name attribute found.");
			continue;
		}

		std::string name = textAttr;
		bool isNotCondition = false;
		if (name.compare("not conditions") == 0) {
			isNotCondition = true;
		}
		else if (name.compare("conditions") != 0) {
			continue;
		}

		textAttr = _property->Attribute("value");
		if (textAttr == nullptr) {
			g_logger->logWarning("WorldReader", "property 'conditions' or 'not conditions' on layer properties has no content.");
			continue;
		}

		std::string conditions = textAttr;

		size_t pos = 0;

		while (!conditions.empty()) {
			if ((pos = conditions.find(",")) == std::string::npos) {
				logError("Layer conditions could not be read, conditions must be two strings separated by a comma (conditionType,conditionName)*");
				continue;
			}

			std::string conditionType = conditions.substr(0, pos);
			conditions.erase(0, pos + 1);
			std::string conditionName;

			if ((pos = conditions.find(",")) != std::string::npos) {
				conditionName = conditions.substr(0, pos);
				conditions.erase(0, pos + 1);
			}
			else {
				conditionName = conditions;
				conditions.clear();
			}

			if (isNotCondition) {
				if (m_core->isConditionFulfilled(conditionType, conditionName)) {
					return false;
				}
			}
			else {
				if (!m_core->isConditionFulfilled(conditionType, conditionName)) {
					return false;
				}
			}
		}

		_property = _property->NextSiblingElement("property");
	}
	return true;
}

void WorldReader::updateData(WorldData& data) const {
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

	// calculate map rect
	data.mapRect.left = 0;
	data.mapRect.top = 0;
	data.mapRect.height = TILE_SIZE_F * data.mapSize.y;
	data.mapRect.width = TILE_SIZE_F * data.mapSize.x;
}
