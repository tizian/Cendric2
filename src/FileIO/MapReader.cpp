#include "FileIO/MapReader.h"
#include "GlobalResource.h"

#define XMLCheckResult(result) if (result != tinyxml2::XML_SUCCESS) {g_logger->logError("MapReader", "XML file could not be read, error: " + std::to_string(static_cast<int>(result))); return false; }

using namespace std;

void MapReader::logError(const std::string& error) const {
	g_logger->logError("MapReader", "Error in map data : " + error);
}

bool MapReader::readMapProperties(tinyxml2::XMLElement* map, WorldData& data_) const {
	if (!WorldReader::readMapProperties(map, data_)) return false;

	// read level properties
	tinyxml2::XMLElement* properties = map->FirstChildElement("properties");
	if (properties == nullptr) {
		logError("XML file could not be read, no properties node found.");
		return false;
	}
	tinyxml2::XMLElement* _property = properties->FirstChildElement("property");

	MapData& data = static_cast<MapData&>(data_);

	const char* textAttr = nullptr;
	while (_property != nullptr) {
		textAttr = nullptr;
		textAttr = _property->Attribute("name");
		if (textAttr == nullptr) {
			logError("XML file could not be read, no property->name attribute found.");
			return false;
		}
		std::string name = textAttr;
		if (name.compare("backgroundlayers") == 0) {
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
		else if (name.compare("weather") == 0) {
			if (!readWeather(_property, data)) return false;
		}
		else if (name.compare("explorable") == 0) {
			data.explorable = true;
		}
		else {
			logError("XML file could not be read, unknown name attribute found in properties (map->properties->property->name).");
			return false;
		}

		_property = _property->NextSiblingElement("property");
	}

	return true;
}

bool MapReader::checkData(MapData& data) const {
	if (!WorldReader::checkData(data)) return false;
	for (auto& it : data.npcs) {
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
	}

	return true;
}


bool MapReader::readCollidableObjectLayer(tinyxml2::XMLElement* objectgroup, MapData& data) const {
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

		// do we we have a rectangle here?
		if (result == tinyxml2::XML_SUCCESS) {
			// rectangle

			int height;
			result = object->QueryIntAttribute("height", &height);
			XMLCheckResult(result);

			data.collidableRects.push_back(sf::FloatRect(
				static_cast<float>(x),
				static_cast<float>(y),
				static_cast<float>(width),
				static_cast<float>(height)));

			object = object->NextSiblingElement("object");
			continue;
		}

		// we do have a polygon here and it should be a triangle. Let's parse those points.
		tinyxml2::XMLElement* polygon = object->FirstChildElement("polygon");
		if (polygon == nullptr) {
			g_logger->logError("MapReader", "XML file could not be read, no polygon tag found on collidable object layer for a not-rectangle object.");
			return false;
		}

		const char* textAttr = nullptr;
		textAttr = polygon->Attribute("points");
		if (textAttr == nullptr) {
			g_logger->logError("MapReader", "XML file could not be read, no polygon->points attribute found for collidable object layer.");
			return false;
		}
		std::string points = textAttr;

		size_t pos = 0;
		sf::Vector2f v1;
		sf::Vector2f v2;
		sf::Vector2f v3;

		// vertex 1
		pos = points.find(",");
		if (pos == std::string::npos) {
			logError("polygon triangle is wrongly defined.");
			return false;
		}
		v1.x = static_cast<float>(atoi(points.substr(0, pos).c_str()) + x);
		points.erase(0, pos + 1);

		pos = points.find(" ");
		if (pos == std::string::npos) {
			logError("polygon triangle is wrongly defined.");
			return false;
		}
		v1.y = static_cast<float>(atoi(points.substr(0, pos).c_str()) + y);
		points.erase(0, pos + 1);

		// vertex 2
		pos = points.find(",");
		if (pos == std::string::npos) {
			logError("polygon triangle is wrongly defined.");
			return false;
		}
		v2.x = static_cast<float>(atoi(points.substr(0, pos).c_str()) + x);
		points.erase(0, pos + 1);

		pos = points.find(" ");
		if (pos == std::string::npos) {
			logError("polygon triangle is wrongly defined.");
			return false;
		}
		v2.y = static_cast<float>(atoi(points.substr(0, pos).c_str()) + y);
		points.erase(0, pos + 1);

		// vertex 3
		pos = points.find(",");
		if (pos == std::string::npos) {
			logError("polygon triangle is wrongly defined.");
			return false;
		}
		v3.x = static_cast<float>(atoi(points.substr(0, pos).c_str()) + x);
		points.erase(0, pos + 1);

		v3.y = static_cast<float>(atoi(points.c_str()) + y);

		data.collidableTriangles.push_back(FloatTriangle(v1, v2, v3));

		object = object->NextSiblingElement("object");
	}
	return true;
}

bool MapReader::readObjects(tinyxml2::XMLElement* map, MapData& data) const {
	tinyxml2::XMLElement* objectgroup = map->FirstChildElement("objectgroup");

	const char* textAttr;
	while (objectgroup != nullptr) {
		if (!layerConditionsFulfilled(objectgroup)) {
			objectgroup = objectgroup->NextSiblingElement("objectgroup");
			continue;
		}
		textAttr = nullptr;
		textAttr = objectgroup->Attribute("name");
		if (textAttr == nullptr) {
			logError("XML file could not be read, no objectgroup->name attribute found.");
			return false;
		}

		std::string name = textAttr;

		if (name.find("npc") != std::string::npos) {
			if (!readNPCs(objectgroup, data)) return false;
		}
		else if (name.find("light") != std::string::npos) {
			if (!readLights(objectgroup, data)) return false;
		}
		else if (name.find("collidable") != std::string::npos) {
			if (!readCollidableObjectLayer(objectgroup, data)) return false;
		}
		else if (name.find("book") != std::string::npos) {
			if (!readBooks(objectgroup, data)) return false;
		}
		else if (name.find("door") != std::string::npos) {
			if (!readDoors(objectgroup, data)) return false;
		}
		else if (name.find("sign") != std::string::npos) {
			if (!readSigns(objectgroup, data)) return false;
		}
		else if (name.find("chest") != std::string::npos) {
			if (!readChests(objectgroup, data)) return false;
		}
		else if (name.find("trigger") != std::string::npos) {
			if (!readTriggers(objectgroup, data)) return false;
		}
		else {
			logError("Objectgroup with unknown name found in map: " + name);
			return false;
		}

		objectgroup = objectgroup->NextSiblingElement("objectgroup");
	}
	return true;
}

bool MapReader::readChests(tinyxml2::XMLElement* objectgroup, WorldData& data) const {
	if (!WorldReader::readChests(objectgroup, data)) return false;

	int offset = static_cast<int>(MapDynamicTileID::Chest) + m_firstGidDynamicTiles - 1;

	for (auto& chest : data.chests) {
		chest.skinNr = (chest.skinNr == 0) ? 0 : ((chest.skinNr - offset) / DYNAMIC_TILE_COUNT);
	}

	return true;
}

bool MapReader::readBooks(tinyxml2::XMLElement* objectgroup, MapData& data) const {
	tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");

	while (object != nullptr) {

		int x;
		tinyxml2::XMLError result = object->QueryIntAttribute("x", &x);
		XMLCheckResult(result);

		int y;
		result = object->QueryIntAttribute("y", &y);
		XMLCheckResult(result);

		int gid;
		result = object->QueryIntAttribute("gid", &gid);
		XMLCheckResult(result);

		int offset = static_cast<int>(MapDynamicTileID::Book) + m_firstGidDynamicTiles - 1;
		int skinNr = (gid == 0) ? 0 : ((gid - offset) / DYNAMIC_TILE_COUNT);

		BookData book;

		book.skinNr = skinNr;
		book.position.x = static_cast<float>(x);
		book.position.y = static_cast<float>(y) - TILE_SIZE_F;

		// book properties
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

				if (attrText.compare("id") == 0) {
					textAttr = nullptr;
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, parsing of book id failed.");
						return false;
					}
					book.id = textAttr;
				}

				_property = _property->NextSiblingElement("property");
			}
		}

		data.books.push_back(book);
		object = object->NextSiblingElement("object");
	}
	return true;
}

bool MapReader::readDoors(tinyxml2::XMLElement* objectgroup, MapData& data) const {
	tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");

	while (object != nullptr) {

		int x;
		tinyxml2::XMLError result = object->QueryIntAttribute("x", &x);
		XMLCheckResult(result);

		int y;
		result = object->QueryIntAttribute("y", &y);
		XMLCheckResult(result);

		int gid;
		result = object->QueryIntAttribute("gid", &gid);
		XMLCheckResult(result);

		int offset = static_cast<int>(MapDynamicTileID::Door) + m_firstGidDynamicTiles - 1;
		int skinNr = (gid == 0) ? 0 : ((gid - offset) / DYNAMIC_TILE_COUNT);

		DoorData door;

		door.skinNr = skinNr;
		door.position.x = static_cast<float>(x);
		door.position.y = static_cast<float>(y) - TILE_SIZE_F;

		// sign properties
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

				// Keys
				if (attrText.compare("key") == 0) {
					std::string keyItemID = _property->Attribute("value");
					if (keyItemID.empty()) {
						logError("XML file could not be read, key itemID is not specified.");
						return false;
					}
					door.keyItemID = keyItemID;
				}

				// Conditions
				std::string name = textAttr;

				bool isNotCondition = false;
				if (name.compare("not conditions") == 0) {
					isNotCondition = true;
				}
				else if (name.compare("conditions") != 0) {
					_property = _property->NextSiblingElement("property");
					continue;
				}

				textAttr = _property->Attribute("value");
				if (textAttr == nullptr) {
					g_logger->logWarning("WorldReader", "property 'conditions' or 'not conditions' on door properties has no content.");
					continue;
				}

				std::string conditions = textAttr;

				size_t pos = 0;

				while (!conditions.empty()) {
					if ((pos = conditions.find(",")) == std::string::npos) {
						logError("Door conditions could not be read, conditions must be two strings separated by a comma (conditionType,conditionName)*");
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

					Condition condition;
					condition.type = conditionType;
					condition.name = conditionName;

					if (isNotCondition) {
						condition.negative = true;
					}
					else {
						condition.negative = false;
					}

					door.conditions.push_back(condition);
				}

				_property = _property->NextSiblingElement("property");
			}
		}

		data.doors.push_back(door);
		object = object->NextSiblingElement("object");
	}
	return true;
}

bool MapReader::readSigns(tinyxml2::XMLElement* objectgroup, WorldData& data) const {
	if (!WorldReader::readSigns(objectgroup, data)) return false;

	int offset = static_cast<int>(MapDynamicTileID::Sign) + m_firstGidDynamicTiles - 1;

	for (auto& sign : data.signs) {
		sign.skinNr = (sign.skinNr == 0) ? 0 : ((sign.skinNr - offset) / DYNAMIC_TILE_COUNT);
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
		npc.position.y = static_cast<float>(y) - TILE_SIZE_F;

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

				if (attrText.compare("id") == 0) {
					textAttr = nullptr;
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value attribute found.");
						return false;
					}
					npc.id = textAttr;
					npc.calculateDefaultFromID();
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
				else if (attrText.compare("texttype") == 0) {
					textAttr = nullptr;
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value attribute found.");
						return false;
					}
					npc.textType = textAttr;
				}
				else if (attrText.compare("dialoguetexture") == 0) {
					textAttr = nullptr;
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value attribute found.");
						return false;
					}
					npc.dialoguetexture = textAttr;
				}
				else if (attrText.compare("light") == 0) {

					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value attribute found.");
						return false;
					}

					std::string value = textAttr;
					LightData lightData;
					if (!resolveLightString(value, lightData)) {
						return false;
					}
					npc.lightData = lightData;
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

bool MapReader::readDynamicTileLayer(const std::string& layer, MapData& data) const {
	std::string layerData = layer;
	size_t pos = layerData.find(",");
	int id;
	int position = 0;
	bool lastIteration = true;
	while (pos != std::string::npos || lastIteration) {
		if (pos == std::string::npos) {
			lastIteration = false;
			id = std::stoi(layerData);
		}
		else {
			id = std::stoi(layerData.substr(0, pos));
			layerData.erase(0, pos + 1);
			pos = layerData.find(",");
		}

		if (id != 0) {
			id -= m_firstGidDynamicTiles;
			MapDynamicTileID tileId = static_cast<MapDynamicTileID>(id % DYNAMIC_TILE_COUNT + 1);
			int skinNr = id / DYNAMIC_TILE_COUNT;
			if (tileId <= MapDynamicTileID::VOID || tileId >= MapDynamicTileID::MAX) {
				logError("Unknown dynamic tile id found on dynamic layer: " + std::to_string(id));
				return false;
			}

			MapDynamicTileData tileData;
			tileData.id = tileId;
			tileData.skinNr = skinNr;
			tileData.spawnPosition = position;

			data.dynamicTiles.push_back(tileData);
		}

		++position;
	}

	return true;
}

bool MapReader::readLayers(tinyxml2::XMLElement* map, MapData& data) const {
	tinyxml2::XMLElement* layer = map->FirstChildElement("layer");

	const char* textAttr;
	while (layer != nullptr) {
		if (!layerConditionsFulfilled(layer)) {
			layer = layer->NextSiblingElement("layer");
			continue;
		}
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
		else if (name.find("dynamic") != std::string::npos) {
			if (!readDynamicTileLayer(layerData, data)) return false;
		}
		else {
			logError("Layer with unknown name found in map.");
			return false;
		}

		layer = layer->NextSiblingElement("layer");
	}
	return true;
}

bool MapReader::readMap(const std::string& filename, MapData& data, const CharacterCore* core) {
	m_core = core;
	tinyxml2::XMLDocument xmlDoc;
	tinyxml2::XMLError result = xmlDoc.LoadFile(getResourcePath(filename).c_str());
	XMLCheckResult(result);

	tinyxml2::XMLElement* map = xmlDoc.FirstChildElement("map");
	if (map == nullptr) {
		logError("XML file could not be read, no map node found.");
		return false;
	}

	if (!readMapProperties(map, data)) return false;
	if (!readFirstGridIDs(map, data)) return false;
	if (!readTileProperties(map, data)) return false;
	if (!readLayers(map, data)) return false;
	if (!readObjects(map, data)) return false;

	updateData(data);
	if (!checkData(data)) return false;
	return true;
}

bool MapReader::readBackgroundTileLayer(const std::string& layer, MapData& data) const {
	std::string layerData = layer;

	size_t pos = 0;
	int x = 0;
	int y = 0;
	std::vector<int> backgroundLayer;
	while ((pos = layerData.find(",")) != std::string::npos) {
		int tileID = std::stoi(layerData.substr(0, pos));

		if (contains(m_tileColliderMap, tileID)) {
			for (auto const& colliderRect : m_tileColliderMap.at(tileID)) {
				sf::FloatRect collider = colliderRect;
				collider.left += x * TILE_SIZE_F;
				collider.top += y * TILE_SIZE_F;
				data.collidableRects.push_back(collider);
			}
		}

		backgroundLayer.push_back(std::stoi(layerData.substr(0, pos)));
		layerData.erase(0, pos + 1);

		if (x + 1 == data.mapSize.x) {
			y++;
			x = 0;
		}
		else {
			x++;
		}
	}

	backgroundLayer.push_back(std::stoi(layerData));

	data.backgroundTileLayers.push_back(backgroundLayer);
	return true;
}

bool MapReader::readCollidableTiles(tinyxml2::XMLElement* firstTile) {
	m_tileColliderMap.clear();

	tinyxml2::XMLElement* tile = firstTile;

	while (tile != nullptr) {
		int tileID;
		tinyxml2::XMLError result = tile->QueryIntAttribute("id", &tileID);
		XMLCheckResult(result);

		tinyxml2::XMLElement* colliders = tile->FirstChildElement("objectgroup");
		if (colliders == nullptr) {
			// this is an animated tile, just move on.
			tile = tile->NextSiblingElement("tile");
			continue;
		}

		m_tileColliderMap.insert({ tileID + 1, std::vector<sf::FloatRect>() });

		tinyxml2::XMLElement* collider = colliders->FirstChildElement("object");
		while (collider != nullptr) {
			sf::FloatRect colliderRect;
			int res;

			result = collider->QueryIntAttribute("x", &res);
			XMLCheckResult(result);
			colliderRect.left = static_cast<float>(res);

			result = collider->QueryIntAttribute("y", &res);
			XMLCheckResult(result);
			colliderRect.top = static_cast<float>(res);

			result = collider->QueryIntAttribute("width", &res);
			XMLCheckResult(result);
			colliderRect.width = static_cast<float>(res);

			result = collider->QueryIntAttribute("height", &res);
			XMLCheckResult(result);
			colliderRect.height = static_cast<float>(res);

			m_tileColliderMap.at(tileID + 1).push_back(colliderRect);

			collider = collider->NextSiblingElement("object");
		}

		tile = tile->NextSiblingElement("tile");
	}

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
		else if (gid == 1) {
			// this is the main tileset
			if (!readCollidableTiles(tileset->FirstChildElement("tile"))) return false;
		}

		tileset = tileset->NextSiblingElement("tileset");
	}

	if (m_firstGidDynamicTiles <= 0) {
		logError("Could not read firstgids, at least one of the required tilesets is missing.");
		return false;
	}
	return true;
}