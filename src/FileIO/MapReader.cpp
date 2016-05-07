#include "FileIO/MapReader.h"
#include "ResourceManager.h"

#define XMLCheckResult(result) if (result != tinyxml2::XML_SUCCESS) {g_logger->logError("MapReader", "XML file could not be read, error: " + std::to_string(static_cast<int>(result))); return false; }

using namespace std;

void MapReader::logError(const std::string& error) const {
	g_logger->logError("MapReader", "Error in map data : " + error);
}

bool MapReader::checkData(MapData& data) const {
	if (!WorldReader::checkData(data)) return false;
	for (size_t i = 0; i < data.dynamicTileLayers.size(); ++i) {
		if (data.dynamicTileLayers[i].first == MapDynamicTileID::VOID) {
			logError("map dynamic tile ID not recognized");
			return false;
		}
		if (data.dynamicTileLayers[i].second.empty() || static_cast<int>(data.dynamicTileLayers[i].second.size()) != data.mapSize.x * data.mapSize.y) {
			logError("dynamic tile layer has not correct size (map size)");
			return false;
		}
	}
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
		if (it.routineID.empty()) {
			g_logger->logWarning("MapReader","a map npc has no routine.");
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
		else if (name.find("sign") != std::string::npos) {
			if (!readSigns(objectgroup, data)) return false;
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
		int skinNr = (gid == 0) ? 0 : ((gid - offset) / DYNAMIC_TILE_COUNT) + 1;

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

				if (attrText.compare("title") == 0) {
					textAttr = nullptr;
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value attribute found.");
						return false;
					}
					book.title = textAttr;
				}
				else if (attrText.find("page") != string::npos) {
					textAttr = nullptr;
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value attribute found.");
						return false;
					}
					
					std::string tex = textAttr;

					BookPage page;

					size_t pos = 0;
					if ((pos = tex.find(",")) == std::string::npos) {
						// this page has only content, no title.
						page.content = tex;
					}
					else {
						// this page has content and a title
						page.title = tex.substr(0, pos);
						tex.erase(0, pos + 1);
						page.content = tex;
					}
					
					book.pages.push_back(page);
				}
				
				_property = _property->NextSiblingElement("property");
			}
		}

		data.books.push_back(book);
		object = object->NextSiblingElement("object");
	}
	return true;
}

bool MapReader::readSigns(tinyxml2::XMLElement* objectgroup, MapData& data) const {
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

		int offset = static_cast<int>(MapDynamicTileID::Sign) + m_firstGidDynamicTiles - 1;
		int skinNr = (gid == 0) ? 0 : ((gid - offset) / DYNAMIC_TILE_COUNT) + 1;

		SignData sign;

		sign.skinNr = skinNr;
		sign.position.x = static_cast<float>(x);
		sign.position.y = static_cast<float>(y) - TILE_SIZE_F;

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

				if (attrText.compare("text") == 0) {
					textAttr = nullptr;
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value attribute found.");
						return false;
					}
					sign.text = textAttr;
				}
				
				_property = _property->NextSiblingElement("property");
			}
		}

		data.signs.push_back(sign);
		object = object->NextSiblingElement("object");
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

				if (attrText.compare("id") == 0) {
					textAttr = nullptr;
					textAttr = _property->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value attribute found.");
						return false;
					}
					npc.id = textAttr;
					npc.dialogueID = std::string(g_resourceManager->getFilename(ResourceID::Npc_folder)) + npc.id + "/dl_" + npc.id + ".lua";
					npc.routineID = std::string(g_resourceManager->getFilename(ResourceID::Npc_folder)) + npc.id + "/ru_" + npc.id + ".lua";
					npc.spritesheetpath = std::string(g_resourceManager->getFilename(ResourceID::Npc_folder)) + npc.id + "/spritesheet_" + npc.id + ".png";
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
		else if (name.find("dynamic cooking") != std::string::npos) {
			if (!readDynamicTileLayer(MapDynamicTileID::Cooking, layerData, data)) return false;
		}
		else if (name.find("dynamic waypoint") != std::string::npos) {
			if (!readDynamicTileLayer(MapDynamicTileID::Waypoint, layerData, data)) return false;
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
	tinyxml2::XMLError result = xmlDoc.LoadFile(getPath(filename).c_str());
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

bool MapReader::readBackgroundTileLayer(const std::string& layer, MapData& data) const {
	std::string layerData = layer;

	size_t pos = 0;
	int x = 0;
	int y = 0;
	std::vector<int> backgroundLayer;
	while ((pos = layerData.find(",")) != std::string::npos) {
		int tileID = std::stoi(layerData.substr(0, pos));

		if (m_tileColliderMap.find(tileID) != m_tileColliderMap.end()) {
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

void MapReader::updateData(MapData& data) const {
	WorldReader::updateData(data);

	// update dynamic tiles
	for (auto& layer : data.dynamicTileLayers) {
		MapDynamicTileID id = layer.first;

		for (int y = 0; y < data.mapSize.y; ++y) {
			for (int x = 0; x < data.mapSize.x; ++x) {
				int skinNr = layer.second[y * data.mapSize.x + x];
				if (skinNr != 0) {
					MapDynamicTileData mdtData;
					mdtData.id = id;
					mdtData.position = sf::Vector2f(x * TILE_SIZE_F, y * TILE_SIZE_F);
					mdtData.skinNr = skinNr;
					mdtData.spawnPosition = y * data.mapSize.x + x;
					data.dynamicTiles.push_back(mdtData);
				}
			}
		}
	}

	for (auto& npc : data.npcs) {
		// why? why does tiled do that to our objects?
		npc.position.y -= TILE_SIZE_F;
	}
}