#include "FileIO/MapReader.h"

#define XMLCheckResult(result) if (result != tinyxml2::XML_SUCCESS) {g_logger->logError("MapReader", "XML file could not be read, error: " + std::to_string(static_cast<int>(result))); return false; }

using namespace std;

void MapReader::logError(const std::string& error) const {
	g_logger->logError("MapReader", "Error in map data : " + error);
}

bool MapReader::checkData(MapData& data) const {
	if (!TMXReader::checkData(data)) return false;
	for (int i = 0; i < data.dynamicTileLayers.size(); i++) {
		if (data.dynamicTileLayers[i].first == MapDynamicTileID::VOID) {
			logError("map dynamic tile ID not recognized");
			return false;
		}
		if (data.dynamicTileLayers[i].second.empty() || data.dynamicTileLayers[i].second.size() != data.mapSize.x * data.mapSize.y) {
			logError("dynamic tile layer has not correct size (map size)");
			return false;
		}
	}
	for (auto it : data.mapExits) {
		if (it.mapExitRect.left < 0.0 || it.mapExitRect.top < 0.0 || it.mapExitRect.left >= data.mapSize.x * data.tileSize.x || it.mapExitRect.top >= data.mapSize.y * data.tileSize.y) {
			logError("a map exit rect is out of range for this map.");
			return false;
		}
		if ((it.mapID.empty() && it.levelID.empty()) || (!it.mapID.empty() && !it.levelID.empty())) {
			logError("map exit map id and level id are both empty or both filled. Only one of them can be set.");
			return false;
		}
		if (it.spawnPoint.x < 0.f || it.spawnPoint.y < 0.f) {
			logError("map exit spawn point is negative.");
			return false;
		}
	}
	for (auto it : data.npcs) {
		if (it.id.empty()) {
			logError("a map npc has no id.");
			return false;
		}
		if (it.objectID == -1) {
			logError("a map npc has no object id.");
			return false;
		}
		if (it.spritesheetpath.empty()) {
			logError("a map npc has no spritesheet path.");
			return false;
		}
		if (it.routineID.empty()) {
			g_logger->logWarning("MapReader","a map npc has no routine.");
		}
	}

	return true;
}

bool MapReader::readMapExits(tinyxml2::XMLElement* objectgroup, MapData& data) const {
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

		MapExitData meData;
		meData.mapExitRect = sf::FloatRect(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height));
		meData.levelID = "";
		meData.mapID = "";
		meData.spawnPoint.x = -1.f;
		meData.spawnPoint.y = -1.f;

		// map spawn point for level exit
		tinyxml2::XMLElement* properties = object->FirstChildElement("properties");
		if (properties == nullptr) {
			g_logger->logError("MapReader", "XML file could not be read, no objectgroup->object->properties attribute found for level exit.");
			return false;
		}

		tinyxml2::XMLElement* _property = properties->FirstChildElement("property");
		while (_property != nullptr) {
			const char* textAttr = nullptr;
			textAttr = _property->Attribute("name");
			if (textAttr == nullptr) {
				g_logger->logError("MapReader", "XML file could not be read, no objectgroup->object->properties->property->name attribute found for level exit.");
				return false;
			}
			std::string name = textAttr;

			if (name.compare("level id") == 0) {
				textAttr = nullptr;
				textAttr = _property->Attribute("value");
				if (textAttr != nullptr) {
					meData.levelID = textAttr;
				}
			}
			else if (name.compare("map id") == 0) {
				textAttr = nullptr;
				textAttr = _property->Attribute("value");
				if (textAttr != nullptr) {
					meData.mapID = textAttr;
				}
			}
			else if (name.compare("x") == 0) {
				tinyxml2::XMLError result = _property->QueryIntAttribute("value", &x);
				XMLCheckResult(result);
				meData.spawnPoint.x = static_cast<float>(x);
			}
			else if (name.compare("y") == 0) {
				tinyxml2::XMLError result = _property->QueryIntAttribute("value", &y);
				XMLCheckResult(result);
				meData.spawnPoint.y = static_cast<float>(y);
			}
			else {
				g_logger->logError("MapReader", "XML file could not be read, unknown objectgroup->object->properties->property->name attribute found for level exit.");
				return false;
			}
			_property = _property->NextSiblingElement("property");
		}
		data.mapExits.push_back(meData);
		object = object->NextSiblingElement("object");
	}
	return true;
}

bool MapReader::readObjects(tinyxml2::XMLElement* map, MapData& data) const {
	tinyxml2::XMLElement* objectgroup = map->FirstChildElement("objectgroup");

	const char* textAttr;
	while (objectgroup != nullptr) {
		textAttr = nullptr;
		textAttr = objectgroup->Attribute("name");
		if (textAttr == nullptr) {
			logError("XML file could not be read, no objectgroup->name attribute found.");
			return false;
		}

		std::string name = textAttr;

		if (name.find("mapexits") != std::string::npos) {
			if (!readMapExits(objectgroup, data)) return false;
		}
		else if (name.find("npc") != std::string::npos) {
			if (!readNPCs(objectgroup, data)) return false;
		}
		else if (name.find("light") != std::string::npos) {
			if (!readLights(objectgroup, data)) return false;
		}
		else {
			logError("Objectgroup with unknown name found in level.");
			return false;
		}

		objectgroup = objectgroup->NextSiblingElement("objectgroup");
	}
	return true;
}

bool MapReader::readNPCs(tinyxml2::XMLElement* objectgroup, MapData& data) const {
	tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");

	while (object != nullptr) {
		int id;
		tinyxml2::XMLError result = object->QueryIntAttribute("id", &id);
		XMLCheckResult(result);

		int x;
		result = object->QueryIntAttribute("x", &x);
		XMLCheckResult(result);

		int y;
		result = object->QueryIntAttribute("y", &y);
		XMLCheckResult(result);

		NPCData npc = DEFAULT_NPC;

		npc.objectID = id;
		npc.position.x = static_cast<float>(x);
		npc.position.y = static_cast<float>(y);

		// npc properties
		tinyxml2::XMLElement* properties = object->FirstChildElement("properties");
		if (properties != nullptr) {
			tinyxml2::XMLElement* _property = properties->FirstChildElement("property");
			while (_property != nullptr) {
				const char* textAttr = nullptr;
				textAttr = _property->Attribute("name");
				if (textAttr == nullptr) {
					logError("XML file could not be read, no objectgroup->object->properties->property->name attribute found.");
					return false;
				}
				std::string attrText = textAttr;

				if (attrText.compare("active") == 0) {
					int active;
					result = _property->QueryIntAttribute("value", &active);
					XMLCheckResult(result);

					npc.talksActive = (active != 0);
				}
				else if (attrText.compare("id") == 0) {
					textAttr = nullptr;
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value attribute found.");
						return false;
					}
					npc.id = textAttr;
				}
				else if (attrText.compare("dialogueid") == 0) {
					textAttr = nullptr;
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value attribute found.");
						return false;
					}
					npc.dialogueID = textAttr;
				}
				else if (attrText.compare("spritesheetpath") == 0) {
					textAttr = nullptr;
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value attribute found.");
						return false;
					}
					npc.spritesheetpath = textAttr;
				}
				else if (attrText.compare("routineid") == 0) {
					textAttr = nullptr;
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value attribute found.");
						return false;
					}
					npc.routineID = textAttr;
				}
				else if (attrText.compare("dialoguetexture") == 0) {
					textAttr = nullptr;
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value attribute found.");
						return false;
					}
					std::string tex = textAttr;

					size_t pos = 0;
					if ((pos = tex.find(",")) == std::string::npos) return false;
					npc.dialogueTexturePositon.left = std::stoi(tex.substr(0, pos));
					tex.erase(0, pos + 1);
					npc.dialogueTexturePositon.top = std::stoi(tex);
				}
				else if (attrText.compare("boundingbox") == 0) {
					textAttr = nullptr;
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value attribute found.");
						return false;
					}
					std::string bb = textAttr;

					size_t pos = 0;
					if ((pos = bb.find(",")) == std::string::npos) return false;
					npc.boundingBox.left = static_cast<float>(std::stoi(bb.substr(0, pos)));
					bb.erase(0, pos + 1);
					if ((pos = bb.find(",")) == std::string::npos) return false;
					npc.boundingBox.top = static_cast<float>(std::stoi(bb.substr(0, pos)));
					bb.erase(0, pos + 1);
					if ((pos = bb.find(",")) == std::string::npos) return false;
					npc.boundingBox.width = static_cast<float>(std::stoi(bb.substr(0, pos)));
					bb.erase(0, pos + 1);
					npc.boundingBox.height = static_cast<float>(std::stoi(bb));
				}

				_property = _property->NextSiblingElement("property");
			}
		}

		data.npcs.push_back(npc);
		object = object->NextSiblingElement("object");
	}
	return true;
}

bool MapReader::readDynamicTileLayer(MapDynamicTileID id, const std::string& layer, MapData& data) const {
	std::string layerData = layer;
	int offset = static_cast<int>(id) + m_firstGidDynamicTiles - 1;
	size_t pos = 0;
	std::vector<int> dynamicTileLayer;
	int skinNr;
	while ((pos = layerData.find(",")) != std::string::npos) {
		skinNr = std::stoi(layerData.substr(0, pos));
		if (skinNr != 0 && ((skinNr - offset) % DYNAMIC_TILE_COUNT) != 0) {
			logError("Dynamic Tile with ID: " + std::to_string(skinNr) + " is not allowed on this layer!");
			return false;
		}
		dynamicTileLayer.push_back(skinNr == 0 ? 0 : ((skinNr - offset) / DYNAMIC_TILE_COUNT) + 1);
		layerData.erase(0, pos + 1);
	}
	skinNr = std::stoi(layerData);
	dynamicTileLayer.push_back(skinNr == 0 ? 0 : ((skinNr - offset) / DYNAMIC_TILE_COUNT) + 1);

	data.dynamicTileLayers.push_back(std::pair<MapDynamicTileID, std::vector<int>>(id, dynamicTileLayer));

	return true;
}

bool MapReader::readLayers(tinyxml2::XMLElement* map, MapData& data) const {
	tinyxml2::XMLElement* layer = map->FirstChildElement("layer");

	const char* textAttr;
	while (layer != nullptr) {
		textAttr = nullptr;
		textAttr = layer->Attribute("name");
		if (textAttr == nullptr) {
			logError("XML file could not be read, no layer->name attribute found.");
			return false;
		}

		std::string name = textAttr;

		tinyxml2::XMLElement* layerDataNode = layer->FirstChildElement("data");
		if (layerDataNode == nullptr) {
			logError("XML file could not be read, no layer->data found.");
			return false;
		}
		std::string layerData = layerDataNode->GetText();

		if (name.find("BG") != std::string::npos || name.find("bg") != std::string::npos) {
			if (!readBackgroundTileLayer(layerData, data)) return false;
		}
		else if (name.find("LFG") != std::string::npos || name.find("lfg") != std::string::npos) {
			if (!readLightedForegroundTileLayer(layerData, data)) return false;
		}
		else if (name.find("FG") != std::string::npos || name.find("fg") != std::string::npos) {
			if (!readForegroundTileLayer(layerData, data)) return false;
		}
		else if (name.find("collidable") != std::string::npos) {
			if (!readCollidableLayer(layerData, data)) return false;
		}
		else if (name.find("dynamic cooking") != std::string::npos) {
			if (!readDynamicTileLayer(MapDynamicTileID::Cooking, layerData, data)) return false;
		}
		else {
			logError("Layer with unknown name found in map.");
			return false;
		}

		layer = layer->NextSiblingElement("layer");
	}
	return true;
}

bool MapReader::readMap(const char* fileName, MapData& data) {
	tinyxml2::XMLDocument xmlDoc;
	tinyxml2::XMLError result = xmlDoc.LoadFile(fileName);
	XMLCheckResult(result);

	tinyxml2::XMLElement* map = xmlDoc.FirstChildElement("map");
	if (map == nullptr) {
		logError("XML file could not be read, no map node found.");
		return false;
	}

	if (!readMapProperties(map, data)) return false;
	if (!readFirstGridIDs(map, data)) return false;
	if (!readAnimatedTiles(map, data)) return false;
	if (!readLayers(map, data)) return false;
	if (!readObjects(map, data)) return false;

	updateData(data);
	if (!checkData(data)) return false;
	return true;
}

bool MapReader::readFirstGridIDs(tinyxml2::XMLElement* map, MapData& data) {
	tinyxml2::XMLElement* tileset = map->FirstChildElement("tileset");

	m_firstGidDynamicTiles = 0;
	const char* textAttr;
	while (tileset != nullptr) {
		textAttr = nullptr;
		textAttr = tileset->Attribute("name");
		if (textAttr == nullptr) {
			logError("XML file could not be read, no tileset->name attribute found.");
			return false;
		}
		std::string name = textAttr;

		int gid;
		tinyxml2::XMLError result = tileset->QueryIntAttribute("firstgid", &gid);
		XMLCheckResult(result);

		if (name.find("dynamic_tiles") != std::string::npos) {
			m_firstGidDynamicTiles = gid;
		}
	
		tileset = tileset->NextSiblingElement("tileset");
	}

	if (m_firstGidDynamicTiles <= 0) {
		logError("Could not read firstgids, at least one of the required tilesets is missing.");
		return false;
	}
	return true;
}

void MapReader::updateData(MapData& data) const {
	TMXReader::updateData(data);

	// update dynamic tiles
	int tileWidth = data.tileSize.x;
	int tileHeight = data.tileSize.y;

	for (auto& layer : data.dynamicTileLayers) {
		MapDynamicTileID id = layer.first;

		for (int y = 0; y < data.mapSize.y; ++y) {
			for (int x = 0; x < data.mapSize.x; ++x) {
				int skinNr = layer.second[y * data.mapSize.x + x];
				if (skinNr != 0) {
					MapDynamicTileData mdtData;
					mdtData.id = id;
					mdtData.position = sf::Vector2f(static_cast<float>(x * tileWidth), static_cast<float>(y * tileHeight));
					mdtData.skinNr = skinNr;
					mdtData.spawnPosition = y * data.mapSize.x + x;
					data.dynamicTiles.push_back(mdtData);
				}
			}
		}
	}

	for (auto& npc : data.npcs) {
		// why? why does tiled do that to our objects?
		npc.position.y -= data.tileSize.y;
	}
}