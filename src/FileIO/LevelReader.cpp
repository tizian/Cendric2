#include "FileIO/LevelReader.h"

#define XMLCheckResult(result) if (result != tinyxml2::XML_SUCCESS) {g_logger->logError("LevelReader", "XML file could not be read, error: " + std::to_string(static_cast<int>(result))); return false; }

LevelReader::LevelReader() : WorldReader() {
	initMaps();
}

LevelReader::~LevelReader() {
	m_levelItemMap.clear();
	m_enemyMap.clear();
}

void LevelReader::initMaps() {
	m_enemyMap.insert({
		{ 1, EnemyID::Rat },
		{ 2, EnemyID::FireRat },
		{ 3, EnemyID::Nekomata_blue },
		{ 4, EnemyID::Crow },
		{ 5, EnemyID::Skeleton },
		{ 6, EnemyID::Gargoyle },
	});
}

void LevelReader::logError(const std::string& error) const {
	g_logger->logError("LevelReader", "Error in level data : " + error);
}

bool LevelReader::readLevel(const std::string& fileName, LevelData& data) {
	tinyxml2::XMLDocument xmlDoc;
	tinyxml2::XMLError result = xmlDoc.LoadFile(fileName.c_str());
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

bool LevelReader::readLevelExits(tinyxml2::XMLElement* objectgroup, LevelData& data) const {
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

		LevelExitData leData;
		leData.levelExitRect = sf::FloatRect(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height));
		leData.mapID = "";
		leData.levelID = "";
		leData.spawnPoint.x = -1.f;
		leData.spawnPoint.y = -1.f;

		// map spawn point for level exit
		tinyxml2::XMLElement* properties = object->FirstChildElement("properties");
		if (properties == nullptr) {
			logError("XML file could not be read, no objectgroup->object->properties attribute found for level exit.");
			return false;
		}

		tinyxml2::XMLElement* _property = properties->FirstChildElement("property");
		while (_property != nullptr) {
			const char* textAttr = nullptr;
			textAttr = _property->Attribute("name");
			if (textAttr == nullptr) {
				logError("XML file could not be read, no objectgroup->object->properties->property->name attribute found for level exit.");
				return false;
			}
			std::string name = textAttr;

			if (name.compare("map id") == 0) {
				textAttr = nullptr;
				textAttr = _property->Attribute("value");
				if (textAttr != nullptr) {
					leData.mapID = textAttr;
				}
			}
			else if (name.compare("level id") == 0) {
				textAttr = nullptr;
				textAttr = _property->Attribute("value");
				if (textAttr != nullptr) {
					leData.levelID = textAttr;
				}
			}
			else if (name.compare("x") == 0) {
				tinyxml2::XMLError result = _property->QueryIntAttribute("value", &x);
				XMLCheckResult(result);
				leData.spawnPoint.x = static_cast<float>(x);
			}
			else if (name.compare("y") == 0) {
				tinyxml2::XMLError result = _property->QueryIntAttribute("value", &y);
				XMLCheckResult(result);
				leData.spawnPoint.y = static_cast<float>(y);
			}
			else {
				logError("XML file could not be read, unknown objectgroup->object->properties->property->name attribute found for level exit.");
				return false;
			}
			_property = _property->NextSiblingElement("property");
		}
		data.levelExits.push_back(leData);
		object = object->NextSiblingElement("object");
	}
	return true;
}

bool LevelReader::readChestTiles(tinyxml2::XMLElement* objectgroup, LevelData& data) const {
	tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");

	while (object != nullptr) {
		int id;
		tinyxml2::XMLError result = object->QueryIntAttribute("id", &id);
		XMLCheckResult(result);

		int gid;
		result = object->QueryIntAttribute("gid", &gid);
		XMLCheckResult(result);

		int x;
		result = object->QueryIntAttribute("x", &x);
		XMLCheckResult(result);

		int y;
		result = object->QueryIntAttribute("y", &y);
		XMLCheckResult(result);

		int offset = static_cast<int>(LevelDynamicTileID::Chest) + m_firstGidDynamicTiles - 1;
		int skinNr = (gid == 0) ? 0 : ((gid - offset) / DYNAMIC_TILE_COUNT) + 1;

		ChestData chestData;
		chestData.skinNr = skinNr;
		chestData.objectID = id;
		chestData.spawnPosition = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));

		// chest loot
		tinyxml2::XMLElement* loot = object->FirstChildElement("properties");
		std::pair<std::map<std::string, int>, int> items;
		items.second = 0;

		if (loot != nullptr) {
			tinyxml2::XMLElement* item = loot->FirstChildElement("property");
			while (item != nullptr) {
				const char* textAttr = nullptr;
				textAttr = item->Attribute("name");
				if (textAttr == nullptr) {
					logError("XML file could not be read, no objectgroup->object->properties->property->name attribute found.");
					return false;
				}
				std::string itemText = textAttr;

				int amount;
				result = item->QueryIntAttribute("value", &amount);
				XMLCheckResult(result);

				if (itemText.compare("gold") == 0 || itemText.compare("Gold") == 0) {
					items.second += amount;
				}
				else if (itemText.compare("strength") == 0 || itemText.compare("Strength") == 0) {
					if (amount < 0 || amount > 4) {
						logError("XML file could not be read, strength attribute for chest is out of bounds (must be between 0 and 4).");
						return false;
					}
					chestData.chestStrength = amount;
				}
				else {
					items.first.insert({ itemText, amount });
				}

				item = item->NextSiblingElement("property");
			}
		}
		chestData.loot = items;
		data.chests.push_back(chestData);

		object = object->NextSiblingElement("object");
	}
	return true;
}

bool LevelReader::readModifierTiles(tinyxml2::XMLElement* objectgroup, LevelData& data) const {
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

		int offset = static_cast<int>(LevelDynamicTileID::Modifier) + m_firstGidDynamicTiles - 1;

		ModifierTileData modifierData;
		modifierData.spawnPosition = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
		modifierData.modifier = EMPTY_SPELLMODIFIER;

		// modifier type and level
		tinyxml2::XMLElement* properties = object->FirstChildElement("properties");

		if (properties != nullptr) {
			tinyxml2::XMLElement* property_ = properties->FirstChildElement("property");
			while (property_ != nullptr) {
				const char* textAttr = nullptr;
				textAttr = property_->Attribute("name");
				if (textAttr == nullptr) {
					logError("XML file could not be read, no objectgroup->object->properties->property->name attribute found.");
					return false;
				}
				std::string propertyText = textAttr;

				if (propertyText.compare("type") == 0) {
					textAttr = property_->Attribute("value");
					if (textAttr == nullptr) {
						logError("Modifer type must be set.");
						return false;
					}
					std::string type = textAttr;
					modifierData.modifier.type = SpellModifier::resolveType(textAttr);
				}
				else {
					int amount;
					result = property_->QueryIntAttribute("value", &amount);
					XMLCheckResult(result);
					if (amount > 3 || amount < 1) {
						logError("Modifier level must be between 1 and 3");
						return false;
					}
					modifierData.modifier.level = amount;
				}

				property_ = property_->NextSiblingElement("property");
			}
		}
		
		data.modifiers.push_back(modifierData);

		object = object->NextSiblingElement("object");
	}
	return true;
}

bool LevelReader::readMovingTiles(tinyxml2::XMLElement* objectgroup, LevelData& data) const {
	tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");

	while (object != nullptr) {
		int id;
		tinyxml2::XMLError result = object->QueryIntAttribute("id", &id);
		XMLCheckResult(result);

		int gid;
		result = object->QueryIntAttribute("gid", &gid);
		XMLCheckResult(result);

		int x;
		result = object->QueryIntAttribute("x", &x);
		XMLCheckResult(result);

		int y;
		result = object->QueryIntAttribute("y", &y);
		XMLCheckResult(result);

		int offset = static_cast<int>(LevelDynamicTileID::Moving) + m_firstGidDynamicTiles - 1;
		int skinNr = (gid == 0) ? 0 : ((gid - offset) / DYNAMIC_TILE_COUNT) + 1;

		MovingTileData movingTileData;
		movingTileData.spawnPosition = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
		movingTileData.skinNr = skinNr;

		// modifier type and level
		tinyxml2::XMLElement* properties = object->FirstChildElement("properties");

		if (properties != nullptr) {
			tinyxml2::XMLElement* property_ = properties->FirstChildElement("property");
			while (property_ != nullptr) {
				const char* textAttr = nullptr;
				textAttr = property_->Attribute("name");
				if (textAttr == nullptr) {
					logError("XML file could not be read, no objectgroup->object->properties->property->name attribute found.");
					return false;
				}
				std::string propertyText = textAttr;

				if (propertyText.compare("frozen") == 0) {
					movingTileData.isFrozen = true;
					property_ = property_->NextSiblingElement("property");
					continue;
				}

				int amount;
				result = property_->QueryIntAttribute("value", &amount);
				XMLCheckResult(result);

				if (propertyText.compare("speed") == 0) {
					if (amount > 1000 || amount < 0) {
						logError("Moving platform speed must be between 0 and 1000");
						return false;
					}
					movingTileData.speed = amount;
				}
				if (propertyText.compare("direction") == 0) {
					movingTileData.initialDirection = amount % 360;
				}
				else if (propertyText.compare("size") == 0 || propertyText.compare("length") == 0) {
					if (amount > 10 || amount < 1) {
						logError("Moving platform size (length in tiles) must be between 1 and 10");
						return false;
					}
					movingTileData.length = amount;
				}
				else if (propertyText.compare("distance") == 0) {
					if (amount > 5000 || amount < 0) {
						logError("Moving platform distance (tiles) must be between 0 and 5000");
						return false;
					}
					movingTileData.distance = amount;
				}

				property_ = property_->NextSiblingElement("property");
			}
		}

		data.movingTiles.push_back(movingTileData);

		object = object->NextSiblingElement("object");
	}
	return true;
}

bool LevelReader::readEnemies(tinyxml2::XMLElement* objectgroup, LevelData& data) const {
	tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");

	while (object != nullptr) {
		int id;
		tinyxml2::XMLError result = object->QueryIntAttribute("id", &id);
		XMLCheckResult(result);

		int gid;
		result = object->QueryIntAttribute("gid", &gid);
		XMLCheckResult(result);

		int x;
		result = object->QueryIntAttribute("x", &x);
		XMLCheckResult(result);

		int y;
		result = object->QueryIntAttribute("y", &y);
		XMLCheckResult(result);

		gid = (gid == 0) ? gid : gid - m_firstGidEnemies + 1;
		if (m_enemyMap.find(gid) == m_enemyMap.end()) {
			logError("Enemy ID not recognized: " + std::to_string(gid));
			return false;
		}

		EnemyData enemyData;
		enemyData.objectID = id;
		enemyData.id = m_enemyMap.at(gid);
		enemyData.spawnPosition = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
		
		// enemy loot
		tinyxml2::XMLElement* properties = object->FirstChildElement("properties");
		std::pair<std::map<std::string, int>, int> items;
		items.second = 0;

		if (properties != nullptr) {
			
			tinyxml2::XMLElement* item = properties->FirstChildElement("property");
			while (item != nullptr) {
				const char* textAttr = nullptr;
				textAttr = item->Attribute("name");
				if (textAttr == nullptr) {
					logError("XML file could not be read, no objectgroup->object->properties->property->name attribute found.");
					return false;
				}
				std::string itemText = textAttr;

				if (itemText.compare("questtarget") == 0) {
					textAttr = item->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, quest target value attribute is void.");
						return false;
					}
					std::string questtargetText = textAttr;
					std::string questID = questtargetText.substr(0, questtargetText.find(","));
					questtargetText.erase(0, questtargetText.find(",") + 1);
					enemyData.questTarget = std::pair<std::string, std::string>(questID, questtargetText);
				}
				else if (itemText.compare("persistent") == 0) {
					enemyData.isPersistent = true;
				}
				else {
					int amount;
					result = item->QueryIntAttribute("value", &amount);
					XMLCheckResult(result);

					if (itemText.compare("gold") == 0 || itemText.compare("Gold") == 0) {
						items.second += amount;
					}
					else {
						items.first.insert({ itemText, amount });
					}
				}

				item = item->NextSiblingElement("property");
			}	
		}
		enemyData.customizedLoot = items;

		data.enemies.push_back(enemyData);
		object = object->NextSiblingElement("object");
	}
	return true;
}

bool LevelReader::readObjects(tinyxml2::XMLElement* map, LevelData& data) const {
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

		if (name.find("levelexits") != std::string::npos) {
			if (!readLevelExits(objectgroup, data)) return false;
		}
		else if (name.find("enemies") != std::string::npos) {
			if (!readEnemies(objectgroup, data)) return false;
		}
		else if (name.find("chests") != std::string::npos) {
			if (!readChestTiles(objectgroup, data)) return false;
		}
		else if (name.find("modifiers") != std::string::npos) {
			if (!readModifierTiles(objectgroup, data)) return false;
		}
		else if (name.find("moving platforms") != std::string::npos) {
			if (!readMovingTiles(objectgroup, data)) return false;
		}
		else if (name.find("light") != std::string::npos) {
			if (!readLights(objectgroup, data)) return false;
		}
		else {
			g_logger->logError("LevelReader", "Objectgroup with unknown name found in level.");
			return false;
		}

		objectgroup = objectgroup->NextSiblingElement("objectgroup");
	}
	return true;
}

bool LevelReader::readLevelItemLayer(const std::string& layer, LevelData& data) const {
	std::string layerData = layer;

	size_t pos = 0;

	int id;
	std::string levelItem;
	while ((pos = layerData.find(",")) != std::string::npos) {
		id = std::stoi(layerData.substr(0, pos));
		id = (id == 0) ? -1 : id - m_firstGidItems;
		layerData.erase(0, pos + 1);
		if (m_levelItemMap.find(id) == m_levelItemMap.end()) {
			logError("Level item ID not recognized: " + std::to_string(id));
			return false;
		}
		levelItem = m_levelItemMap.at(id);
		data.levelItems.push_back(levelItem);
	}
	id = std::stoi(layerData);
	id = (id == 0) ? -1 : id - m_firstGidItems;
	if (m_levelItemMap.find(id) == m_levelItemMap.end()) {
		logError("Level item ID not recognized: " + std::to_string(id));
		return false;
	}
	levelItem = m_levelItemMap.at(id);
	data.levelItems.push_back(levelItem);

	return true;
}

bool LevelReader::readDynamicTileLayer(LevelDynamicTileID id, const std::string& layer, LevelData& data) const {
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

	data.dynamicTileLayers.push_back(std::pair<LevelDynamicTileID, std::vector<int>>(id, dynamicTileLayer));

	return true;
}

bool LevelReader::readLeverLayer(const std::string& layer, LevelData& data) const {
	std::string layerData = layer;
	int leverOffset = static_cast<int>(LevelDynamicTileID::Lever) + m_firstGidDynamicTiles - 1;
	int onOffset = static_cast<int>(LevelDynamicTileID::SwitchableOn) + m_firstGidDynamicTiles - 1;
	int offOffset = static_cast<int>(LevelDynamicTileID::SwitchableOff) + m_firstGidDynamicTiles - 1;

	size_t pos = 0;
	LeverData leData;

	int skinNr;
	int x = 0;
	int y = 0;
	while ((pos = layerData.find(",")) != std::string::npos) {
		skinNr = std::stoi(layerData.substr(0, pos));
		if (skinNr == 0) {
			// ok, continue
		}
		else if ((skinNr - leverOffset) % DYNAMIC_TILE_COUNT == 0) {
			// we've found a lever!
			LevelDynamicTileData lever;
			lever.id = LevelDynamicTileID::Lever;
			lever.position = sf::Vector2f(x * TILE_SIZE_F, y * TILE_SIZE_F);
			lever.skinNr = ((skinNr - leverOffset) / DYNAMIC_TILE_COUNT) + 1;
			lever.spawnPosition = y * data.mapSize.x + x;
			leData.levers.push_back(lever);
		}
		else if ((skinNr - onOffset) % DYNAMIC_TILE_COUNT == 0) {
			LevelDynamicTileData switchTile;
			switchTile.id = LevelDynamicTileID::SwitchableOn;
			switchTile.position = sf::Vector2f(x * TILE_SIZE_F, y * TILE_SIZE_F);
			switchTile.skinNr = ((skinNr - onOffset) / DYNAMIC_TILE_COUNT) + 1;
			switchTile.spawnPosition = y * data.mapSize.x + x;
			leData.dependentTiles.push_back(switchTile);
		}
		else if ((skinNr - offOffset) % DYNAMIC_TILE_COUNT == 0) {
			LevelDynamicTileData switchTile;
			switchTile.id = LevelDynamicTileID::SwitchableOff;
			switchTile.position = sf::Vector2f(x * TILE_SIZE_F, y * TILE_SIZE_F);
			switchTile.skinNr = ((skinNr - offOffset) / DYNAMIC_TILE_COUNT) + 1;
			switchTile.spawnPosition = y * data.mapSize.x + x;
			leData.dependentTiles.push_back(switchTile);
		}
		else {
			logError("Wrong tile id found on a lever layer, id=" + std::to_string(skinNr));
			return false;
		}

		layerData.erase(0, pos + 1);

		if (x + 1 >= data.mapSize.x) {
			x = 0;
			y++;
		}
		else {
			x++;
		}
	}
	
	data.levers.push_back(leData);

	return true;
}

bool LevelReader::readLayers(tinyxml2::XMLElement* map, LevelData& data) const {
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
		else if (name.find("dynamic lever") != std::string::npos) {
			if (!readLeverLayer(layerData, data)) return false;
		}
		else if (name.find("dynamic checkpoint") != std::string::npos) {
			if (!readDynamicTileLayer(LevelDynamicTileID::Checkpoint, layerData, data)) return false;
		}
		else if (name.find("dynamic ice") != std::string::npos) {
			if (!readDynamicTileLayer(LevelDynamicTileID::Ice, layerData, data)) return false;
		}
		else if (name.find("dynamic spikestop") != std::string::npos) {
			if (!readDynamicTileLayer(LevelDynamicTileID::SpikesTop, layerData, data)) return false;
		}
		else if (name.find("dynamic spikesbottom") != std::string::npos) {
			if (!readDynamicTileLayer(LevelDynamicTileID::SpikesBottom, layerData, data)) return false;
		}
		else if (name.find("dynamic torch") != std::string::npos) {
			if (!readDynamicTileLayer(LevelDynamicTileID::Torch, layerData, data)) return false;
		}
		else if (name.find("dynamic destructible") != std::string::npos) {
			if (!readDynamicTileLayer(LevelDynamicTileID::Destructible, layerData, data)) return false;
		}
		else if (name.find("dynamic water") != std::string::npos) {
			if (!readDynamicTileLayer(LevelDynamicTileID::Water, layerData, data)) return false;
		}
		else if (name.find("dynamic shiftable") != std::string::npos) {
			if (!readDynamicTileLayer(LevelDynamicTileID::Shiftable, layerData, data)) return false;
		}
		else if (name.find("collidable") != std::string::npos) {
			if (!readCollidableLayer(layerData, data)) return false;
		}
		else if (name.find("items") != std::string::npos) {
			if (!readLevelItemLayer(layerData, data)) return false;
		}
		else {
			g_logger->logError("LevelReader", "Layer with unknown name found in level.");
			return false;
		}

		layer = layer->NextSiblingElement("layer");
	}
	return true;
}

bool LevelReader::readItemIDs(tinyxml2::XMLElement* _tile) {
	m_levelItemMap.clear();
	m_levelItemMap.insert({ -1, "" });
	tinyxml2::XMLElement* tile = _tile;

	while (tile != nullptr) {
		int tileID;
		tinyxml2::XMLError result = tile->QueryIntAttribute("id", &tileID);
		XMLCheckResult(result);

		tinyxml2::XMLElement* properties = tile->FirstChildElement("properties");
		if (properties == nullptr) {
			logError("Could not read item tile properties, no tileset->tile->properties tag found.");
			return false;
		}
		tinyxml2::XMLElement* _property = properties->FirstChildElement("property");
		if (_property == nullptr) {
			logError("Could not read item tile properties, no tileset->tile->properties->property tag found.");
			return false;
		}
		const char* textAttr = nullptr;
		textAttr = _property->Attribute("name");
		if (textAttr == nullptr) {
			logError("XML file could not be read, no tileset->tile->properties->property name attribute found.");
			return false;
		}
		std::string name = textAttr;
		if (name.compare("id") != 0) {
			logError("XML file could not be read, wrong tile property (not \"id\").");
			return false;
		}
		textAttr = nullptr;
		textAttr = _property->Attribute("value");
		if (textAttr == nullptr) {
			logError("XML file could not be read, no tileset->tile->properties->property value attribute found.");
			return false;
		}

		m_levelItemMap.insert({ tileID, std::string(textAttr) });

		tile = tile->NextSiblingElement("tile");
	}

	return true;
}

bool LevelReader::readFirstGridIDs(tinyxml2::XMLElement* map, LevelData& data) {
	tinyxml2::XMLElement* tileset = map->FirstChildElement("tileset");

	m_firstGidDynamicTiles = 0;
	m_firstGidEnemies = 0;
	m_firstGidItems = 0;
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
		else if (name.find("enemies") != std::string::npos) {
			m_firstGidEnemies = gid;
		}
		else if (name.find("levelitems") != std::string::npos) {
			m_firstGidItems = gid;
			if (!readItemIDs(tileset->FirstChildElement("tile"))) return false;
		}

		tileset = tileset->NextSiblingElement("tileset");
	}

	if (m_firstGidItems <= 0 || m_firstGidDynamicTiles <= 0 || m_firstGidEnemies <= 0) {
		logError("Could not read firstgids, at least one of the required tilesets is missing.");
		return false;
	}
	return true;
}

bool LevelReader::readBackgroundLayers(tinyxml2::XMLElement* _property, WorldData& data_) const {
	// we've found the property "backgroundlayers"
	LevelData* data = static_cast<LevelData*>(&data_);
	if (data == nullptr) return false;
	const char* textAttr = nullptr;
	textAttr = _property->Attribute("value");
	if (textAttr == nullptr) {
		logError("XML file could not be read, no value attribute found (map->properties->property->name=backgroundlayer).");
		return false;
	}
	std::string backgroundLayerText = textAttr;

	size_t pos = 0;
	float distance = 0.f;
	std::string backgroundLayer = "";
	while ((pos = backgroundLayerText.find(",")) != std::string::npos) {
		distance = static_cast<float>(atof(backgroundLayerText.substr(0, pos).c_str()));
		backgroundLayerText.erase(0, pos + 1);
		if ((pos = backgroundLayerText.find(",")) != std::string::npos) {
			backgroundLayer = backgroundLayerText.substr(0, pos);
			backgroundLayerText.erase(0, pos + 1);
		}
		else {
			backgroundLayer = backgroundLayerText;
			backgroundLayerText.clear();
		}
		BackgroundLayer layer;
		layer.load(backgroundLayer, distance);
		data->backgroundLayers.push_back(layer);
	}

	return true;
}

void LevelReader::updateData(LevelData& data)  const {
	WorldReader::updateData(data);

	// calculate dynamic tiles
	for (auto& layer : data.dynamicTileLayers) {
		LevelDynamicTileID id = layer.first;

		if (id == LevelDynamicTileID::Water) {
			// Read in DynamicWaterTiles: Look for n x m tile rectangles inside layer

			std::vector<bool> processed(layer.second.size(), false);
			LevelDynamicTileData ldtData;
			ldtData.id = id;

			for (int y = 0; y < data.mapSize.y; ++y) {
				for (int x = 0; x < data.mapSize.x; ++x) {
					int skinNr = layer.second[y * data.mapSize.x + x];

					if (skinNr != 0 && !processed[y * data.mapSize.x + x]) {		// found start of unprocessed rectangle
						int x0 = x; int xi = x0;
						int y0 = y; int yi = y0;

						int width = 0;
						int height = 0;

						// Check size of rectangle in both x and y
						while (xi < data.mapSize.x && layer.second[y0 * data.mapSize.x + xi] == skinNr && !processed[y0 * data.mapSize.x + xi]) {
							width++;
							xi++;
						}
						while (yi < data.mapSize.y && layer.second[yi * data.mapSize.x + x0] == skinNr && !processed[yi * data.mapSize.x + x0]) {
							height++;
							yi++;
						}

						// Mark whole rectangle as processed
						for (int xi = x0; xi < x0 + width; ++xi) {
							for (int yi = y0; yi < y0 + height; ++yi) {
								processed[yi * data.mapSize.x + xi] = true;
							}
						}

						// Fill in info
						ldtData.position = sf::Vector2f(x * TILE_SIZE_F, y * TILE_SIZE_F);
						ldtData.skinNr = skinNr;
						ldtData.spawnPosition = y * data.mapSize.x + x;
						ldtData.size = sf::Vector2f(width * TILE_SIZE_F, height * TILE_SIZE_F);
						data.dynamicTiles.push_back(ldtData);
					}
				}
			}
		}
		else {
			// normal tiles
			for (int y = 0; y < data.mapSize.y; ++y) {
				for (int x = 0; x < data.mapSize.x; ++x) {
					int skinNr = layer.second[y * data.mapSize.x + x];
					if (skinNr != 0) {
						LevelDynamicTileData ldeData;
						ldeData.id = id;
						ldeData.position = sf::Vector2f(x * TILE_SIZE_F, y * TILE_SIZE_F);
						ldeData.skinNr = skinNr;
						ldeData.spawnPosition = y * data.mapSize.x + x;
						data.dynamicTiles.push_back(ldeData);
					}
				}
			}
		}
	}
}

bool LevelReader::checkData(LevelData& data) const {
	if (!WorldReader::checkData(data)) return false;
	for (int i = 0; i < data.dynamicTileLayers.size(); i++) {
		if (data.dynamicTileLayers[i].first == LevelDynamicTileID::VOID) {
			logError("level dynamic tile ID not recognized");
			return false;
		}
		if (data.dynamicTileLayers[i].second.empty() || data.dynamicTileLayers[i].second.size() != data.mapSize.x * data.mapSize.y) {
			logError("dynamic tile layer has not correct size (map size)");
			return false;
		}
	}
	for (auto& it : data.levelExits) {
		if (it.levelExitRect.height <= 0.f || it.levelExitRect.width <= 0.f) {
			logError("level exit rectangle has volume negative or null.");
			return false;
		}
		if ((it.mapID.empty() && it.levelID.empty()) || (!it.mapID.empty() && !it.levelID.empty())) {
			logError("level exit map id and level id are both empty or both filled. Only one of them can be set.");
			return false;
		}
		if (it.spawnPoint.x < 0.f || it.spawnPoint.y < 0.f) {
			logError("level exit spawn point is negative.");
			return false;
		}
	}
	for (auto& it : data.modifiers) {
		if (it.modifier.level == 0) {
			logError("modifier tile level is not set.");
			return false;
		}
		if (it.modifier.type == SpellModifierType::VOID) {
			logError("modifier tile type is not set.");
			return false;
		}
	}
	for (auto& it : data.movingTiles) {
		if (it.initialDirection == -1) {
			logError("Moving tile initial direction must be set.");
			return false;
		}
		if (it.speed == 0.f) {
			g_logger->logWarning("LevelReader","Moving tile speed is 0.");
		}
		if (it.distance == 0) {
			g_logger->logWarning("LevelReader", "Moving tile distance is 0.");
		}
	}
	for (auto& it : data.enemies) {
		if (!it.questTarget.first.empty() && it.questTarget.second.empty()) {
			logError("enemy quest target name must not must not be empty when quest id is filled.");
			return false;
		}
		if (it.isPersistent && (!it.questTarget.first.empty() || !it.customizedLoot.first.empty() || it.customizedLoot.second != 0)) {
			g_logger->logWarning("LevelReader", "a persistent enemy cannot have customized loot or be a quest target.");
			it.questTarget.first.clear();
			it.questTarget.second.clear();
			it.customizedLoot.first.clear();
			it.customizedLoot.second = 0;
		}
	}
	if (data.levelItems.size() != data.mapSize.x * data.mapSize.y) {
		logError("level item layer has not correct size (map size)");
		return false;
	}

	return true;
}