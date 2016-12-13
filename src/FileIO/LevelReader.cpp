#include "FileIO/LevelReader.h"

#define XMLCheckResult(result) if (result != tinyxml2::XML_SUCCESS) {g_logger->logError("LevelReader", "XML file could not be read, error: " + std::to_string(static_cast<int>(result))); return false; }

LevelReader::LevelReader() : WorldReader() {
}

LevelReader::~LevelReader() {
	m_levelItemMap.clear();
}

void LevelReader::logError(const std::string& error) const {
	g_logger->logError("LevelReader", "Error in level data : " + error);
}

bool LevelReader::readLevel(const std::string& fileName, LevelData& data, const CharacterCore* core) {
	m_core = core;
	tinyxml2::XMLDocument xmlDoc;
	tinyxml2::XMLError result = xmlDoc.LoadFile(getResourcePath(fileName).c_str());
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
		int skinNr = (gid == 0) ? 0 : ((gid - offset) / DYNAMIC_TILE_COUNT);

		ChestTileData chestData;
		chestData.skinNr = skinNr;
		chestData.objectID = id;
		chestData.spawnPosition = sf::Vector2f(static_cast<float>(x), static_cast<float>(y) - TILE_SIZE_F);

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

				if (itemText.compare("permanent") == 0) {
					chestData.isPermanent = true;
				}
				else if (itemText.compare("open") == 0) {
					chestData.isOpen = true;
				}
				else if (itemText.compare("text") == 0) {

					textAttr = item->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value attribute found.");
						return false;
					}
					chestData.tooltipText = textAttr;

				}
				else if (itemText.compare("light") == 0) {

					textAttr = item->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value attribute found.");
						return false;
					}
					
					std::string value = textAttr;
					LightData lightData;
					if (!resolveLightString(value, lightData)) {
						return false;
					}
					chestData.lightData = lightData;
				}
				else if (itemText.compare("gold") == 0) {
					int amount;
					result = item->QueryIntAttribute("value", &amount);
					XMLCheckResult(result);

					items.second += amount;
				}
				else if (itemText.compare("strength") == 0) {
					int amount;
					result = item->QueryIntAttribute("value", &amount);
					XMLCheckResult(result);

					if (amount < 0 || amount > 5) {
						logError("XML file could not be read, strength attribute for chest is out of bounds (must be between 0 and 5).");
						return false;
					}
					chestData.chestStrength = amount;
				}
				else if (itemText.compare("key") == 0) {
					std::string keyItemID = item->Attribute("value");
					if (keyItemID.empty()) {
						logError("XML file could not be read, key itemID is not specified.");
						return false;
					}
					chestData.keyItemID = keyItemID;
				}
				else if (itemText.compare("condition progress") == 0) {

					std::string conditionProgress = item->Attribute("value");
					if (conditionProgress.empty()) {
						logError("XML file could not be read, chest condition is empty.");
						return false;
					}

					size_t pos = 0;
					if ((pos = conditionProgress.find(",")) == std::string::npos) {
						logError("XML file could not be read, chest condition progress value must be two strings, seperated by a comma.");
						return false;
					}

					chestData.conditionProgress.type = conditionProgress.substr(0, pos);
					conditionProgress.erase(0, pos + 1);
					chestData.conditionProgress.name = conditionProgress;
				}
				else {
					int amount;
					result = item->QueryIntAttribute("value", &amount);
					XMLCheckResult(result);

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

		ModifierTileData modifierData;
		modifierData.spawnPosition = sf::Vector2f(static_cast<float>(x), static_cast<float>(y) - TILE_SIZE_F);
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
	int leverOffset = static_cast<int>(LevelDynamicTileID::Lever) + m_firstGidDynamicTiles - 1;
	LeverData leData;

	while (object != nullptr) {

		int gid;
		tinyxml2::XMLError result = object->QueryIntAttribute("gid", &gid);
		XMLCheckResult(result);

		int x;
		result = object->QueryIntAttribute("x", &x);
		XMLCheckResult(result);

		int y;
		result = object->QueryIntAttribute("y", &y);
		XMLCheckResult(result);

		if ((gid - leverOffset) % DYNAMIC_TILE_COUNT == 0) {
			// this is a lever on a moving tile layer
			LevelDynamicTileData lever;
			lever.id = LevelDynamicTileID::Lever;
			lever.position = sf::Vector2f(static_cast<float>(x), static_cast<float>(y) - TILE_SIZE_F);
			lever.skinNr = ((gid - leverOffset) / DYNAMIC_TILE_COUNT);
			lever.spawnPosition = gid; // its an object now
			leData.levers.push_back(lever);

			object = object->NextSiblingElement("object");
			continue;
		}

		int offset = static_cast<int>(LevelDynamicTileID::Moving) + m_firstGidDynamicTiles - 1;
		int skinNr = (gid == 0) ? 0 : ((gid - offset) / DYNAMIC_TILE_COUNT);

		MovingTileData movingTileData;
		movingTileData.spawnPosition = sf::Vector2f(static_cast<float>(x), static_cast<float>(y) - TILE_SIZE_F);
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
				if (propertyText.compare("inactive") == 0) {
					movingTileData.isActive = false;
					property_ = property_->NextSiblingElement("property");
					continue;
				}
				if (propertyText.compare("oneway") == 0) {
					movingTileData.isOneWay = true;
					property_ = property_->NextSiblingElement("property");
					continue;
				}
				if (propertyText.compare("direction") == 0) {
					textAttr = property_->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value (of name 'direction') attribute found.");
						return false;
					}
					std::string initialDirectionString = textAttr;
					try {
						int initialDirection = std::stoi(textAttr);
						movingTileData.initialDirection = initialDirection % 360;
						property_ = property_->NextSiblingElement("property");
						continue;
					}
					catch (const std::exception& e) { 
						logError("Invalid moving tile direction, conversion to integer failed: " + std::string(e.what()) + " Direction attribute: " + initialDirectionString);
						return false;
					}
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

		leData.dependentMovingTiles.push_back(movingTileData);

		object = object->NextSiblingElement("object");
	}

	data.levers.push_back(leData);
	return true;
}

bool LevelReader::readDoorTiles(tinyxml2::XMLElement* objectgroup, LevelData& data) const {
	tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");
	int offset = static_cast<int>(LevelDynamicTileID::Door) + m_firstGidDynamicTiles - 1;

	while (object != nullptr) {

		int gid;
		tinyxml2::XMLError result = object->QueryIntAttribute("gid", &gid);
		XMLCheckResult(result);

		int x;
		result = object->QueryIntAttribute("x", &x);
		XMLCheckResult(result);

		int y;
		result = object->QueryIntAttribute("y", &y);
		XMLCheckResult(result);

		if ((gid - offset) % DYNAMIC_TILE_COUNT != 0) {
			logError("Wrong tile on door layer, gid=" + std::to_string(gid));
			return false;
		}

		DoorData doorData;
		doorData.position = sf::Vector2f(static_cast<float>(x), static_cast<float>(y) - TILE_SIZE_F);
		doorData.skinNr = ((gid - offset) / DYNAMIC_TILE_COUNT);

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

				if (propertyText.compare("collidable") == 0) {
					doorData.isCollidable = true;
					property_ = property_->NextSiblingElement("property");
					continue;
				}
				
				if (propertyText.compare("key") == 0) {
					std::string keyItemID = property_->Attribute("value");
					if (keyItemID.empty()) {
						logError("XML file could not be read, door key itemID is not specified.");
						return false;
					}
					doorData.keyItemID = keyItemID;
					property_ = property_->NextSiblingElement("property");
					continue;
				}

				int amount;
				result = property_->QueryIntAttribute("value", &amount);
				XMLCheckResult(result);

				if (propertyText.compare("width") == 0) {
					if (amount < 1 || amount > 2) {
						logError("Door tile width must be between 1 and 2");
						return false;
					}
					doorData.tileWidth = amount;
				}
				else if (propertyText.compare("strength") == 0) {
					if (amount < 0 || amount > 4) {
						logError("Door tile strength must be between 0 and 4");
						return false;
					}
					doorData.strength = amount;
				}
				
				property_ = property_->NextSiblingElement("property");
			}
		}

		object = object->NextSiblingElement("object");
		data.doors.push_back(doorData);
	}

	return true;
}

bool LevelReader::readJumpingTiles(tinyxml2::XMLElement* objectgroup, LevelData& data) const {
	tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");

	while (object != nullptr) {

		int gid;
		tinyxml2::XMLError result = object->QueryIntAttribute("gid", &gid);
		XMLCheckResult(result);

		int x;
		result = object->QueryIntAttribute("x", &x);
		XMLCheckResult(result);

		int y;
		result = object->QueryIntAttribute("y", &y);
		XMLCheckResult(result);

		int offset = static_cast<int>(LevelDynamicTileID::Jumping) + m_firstGidDynamicTiles - 1;
		int skinNr = (gid == 0) ? 0 : ((gid - offset) / DYNAMIC_TILE_COUNT);

		JumpingTileData jumpingTileData;
		jumpingTileData.spawnPosition = sf::Vector2f(static_cast<float>(x), static_cast<float>(y) - TILE_SIZE_F);
		jumpingTileData.skinNr = skinNr;

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

				if (propertyText.compare("alternating") == 0) {
					jumpingTileData.isAlternating = true;
					property_ = property_->NextSiblingElement("property");
					continue;
				}
				if (propertyText.compare("aggro") == 0) {
					jumpingTileData.isAggro = true;
					property_ = property_->NextSiblingElement("property");
					continue;
				}
				if (propertyText.compare("direction") == 0) {
					textAttr = property_->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value (of name 'direction') attribute found.");
						return false;
					}
					std::string directionString = textAttr;
					try {
						int direction = std::stoi(textAttr);

						if (std::abs(direction) >= 90) {
							logError("Invalid jumping tile direction, you can only use upwards directions (direction < 90 && direction > -90");
							return false;
						}
						jumpingTileData.direction = direction % 360;
						property_ = property_->NextSiblingElement("property");
						continue;
					}
					catch (const std::exception& e) {
						logError("Invalid jumping tile direction, conversion to integer failed: " + std::string(e.what()) + " Direction attribute: " + directionString);
						return false;
					}
				}

				int amount;
				result = property_->QueryIntAttribute("value", &amount);
				XMLCheckResult(result);
				if (propertyText.compare("velocity") == 0) {
					if (amount > 1000 || amount < 100) {
						logError("jumping tile velocity must be between 200 and 1000");
						return false;
					}
					jumpingTileData.velocity = amount;
				}
				else if (propertyText.compare("waiting") == 0) {
					if (amount > 10000 || amount < 100) {
						logError("waiting time (ms) for jumping tile must be between 100 and 10000");
						return false;
					}
					jumpingTileData.waitingTime = amount;
				}
				

				property_ = property_->NextSiblingElement("property");
			}
		}

		data.jumpingTiles.push_back(jumpingTileData);

		object = object->NextSiblingElement("object");
	}
	return true;
}

bool LevelReader::readLadderTiles(tinyxml2::XMLElement* objectgroup, LevelData& data) const {
	tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");

	while (object != nullptr) {

		int gid;
		tinyxml2::XMLError result = object->QueryIntAttribute("gid", &gid);
		XMLCheckResult(result);

		int x;
		result = object->QueryIntAttribute("x", &x);
		XMLCheckResult(result);

		int y;
		result = object->QueryIntAttribute("y", &y);
		XMLCheckResult(result);

		int offset = static_cast<int>(LevelDynamicTileID::Ladder) + m_firstGidDynamicTiles - 1;
		int skinNr = (gid == 0) ? 0 : ((gid - offset) / DYNAMIC_TILE_COUNT);

		LadderTileData ladderData;
		ladderData.position = sf::Vector2f(static_cast<float>(x), static_cast<float>(y) - TILE_SIZE_F);
		ladderData.skinNr = skinNr;

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

				if (propertyText.compare("size") == 0) {
					int amount;
					result = property_->QueryIntAttribute("value", &amount);
					XMLCheckResult(result);

					if (amount > 100 || amount < 2) {
						logError("ladder size (height in tiles) must be between 2 and 100");
						return false;
					}

					ladderData.size = amount;
				}

				property_ = property_->NextSiblingElement("property");
			}
		}

		data.ladderTiles.push_back(ladderData);

		object = object->NextSiblingElement("object");
	}
	return true;
}

bool LevelReader::readSignTiles(tinyxml2::XMLElement* objectgroup, LevelData& data) const {
	tinyxml2::XMLElement* object = objectgroup->FirstChildElement("object");

	while (object != nullptr) {

		int gid;
		tinyxml2::XMLError result = object->QueryIntAttribute("gid", &gid);
		XMLCheckResult(result);

		int x;
		result = object->QueryIntAttribute("x", &x);
		XMLCheckResult(result);

		int y;
		result = object->QueryIntAttribute("y", &y);
		XMLCheckResult(result);

		int offset = static_cast<int>(LevelDynamicTileID::Sign) + m_firstGidDynamicTiles - 1;
		int skinNr = (gid == 0) ? 0 : ((gid - offset) / DYNAMIC_TILE_COUNT);

		SignData sign;
		sign.skinNr = skinNr;
		sign.position.x = static_cast<float>(x);
		sign.position.y = static_cast<float>(y) - TILE_SIZE_F;

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
				std::string attrText = textAttr;

				if (attrText.compare("text") == 0) {
					textAttr = nullptr;
					textAttr = property_->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, no objectgroup->object->properties->property->value attribute found.");
						return false;
					}
					sign.text = textAttr;
				}

				property_ = property_->NextSiblingElement("property");
			}
		}

		data.signTiles.push_back(sign);

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
		EnemyID enemyId = static_cast<EnemyID>(gid);
		if (enemyId <= EnemyID::VOID || enemyId >= EnemyID::MAX) {
			logError("Enemy ID not recognized: " + std::to_string(gid));
			return false;
		}

		EnemyData enemyData;
		enemyData.objectID = id;
		enemyData.id = enemyId;
		enemyData.spawnPosition = sf::Vector2f(static_cast<float>(x), static_cast<float>(y) - TILE_SIZE_F);
		
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

				if (itemText.find("questtarget") != std::string::npos) {
					textAttr = item->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, quest target value attribute is empty.");
						return false;
					}
					std::string questtargetText = textAttr;
					std::string questID = questtargetText.substr(0, questtargetText.find(","));
					questtargetText.erase(0, questtargetText.find(",") + 1);
					enemyData.questTargets.push_back(std::pair<std::string, std::string>(questID, questtargetText));
					enemyData.isUnique = true;
				}
				else if (itemText.compare("questcondition") == 0) {
					textAttr = item->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, quest condition value attribute is empty.");
						return false;
					}
					std::string questcondition = textAttr;
					std::string questID = questcondition.substr(0, questcondition.find(","));
					questcondition.erase(0, questcondition.find(",") + 1);
					enemyData.questCondition = std::pair<std::string, std::string>(questID, questcondition);
					enemyData.isUnique = true;
				}
				else if (itemText.compare("unique") == 0) {
					enemyData.isUnique = true;
				}
				else if (itemText.compare("boss") == 0) {
					enemyData.isBoss = true;
				}
				else if (itemText.compare("skinnr") == 0) {
					int skinNr;
					result = item->QueryIntAttribute("value", &skinNr);
					XMLCheckResult(result);

					enemyData.skinNr = skinNr;
				}
				else if (itemText.compare("luapath") == 0) {
					textAttr = item->Attribute("value");
					if (textAttr == nullptr) {
						logError("XML file could not be read, luapath value attribute is empty.");
						return false;
					}
					
					enemyData.luaPath = textAttr;
				}
				else {
					int amount;
					result = item->QueryIntAttribute("value", &amount);
					XMLCheckResult(result);

					if (itemText.compare("gold") == 0) {
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

		if (name.find("enemy") != std::string::npos) {
			if (!readEnemies(objectgroup, data)) return false;
		}
		else if (name.find("chest") != std::string::npos) {
			if (!readChestTiles(objectgroup, data)) return false;
		}
		else if (name.find("modifier") != std::string::npos) {
			if (!readModifierTiles(objectgroup, data)) return false;
		}
		else if (name.find("moving platform") != std::string::npos) {
			if (!readMovingTiles(objectgroup, data)) return false;
		}
		else if (name.find("jumping") != std::string::npos) {
			if (!readJumpingTiles(objectgroup, data)) return false;
		}
		else if (name.find("sign") != std::string::npos) {
			if (!readSignTiles(objectgroup, data)) return false;
		}
		else if (name.find("ladder") != std::string::npos) {
			if (!readLadderTiles(objectgroup, data)) return false;
		}
		else if (name.find("light") != std::string::npos) {
			if (!readLights(objectgroup, data)) return false;
		}
		else if (name.find("trigger") != std::string::npos) {
			if (!readTriggers(objectgroup, data)) return false;
		}
		else if (name.find("door") != std::string::npos) {
			if (!readDoorTiles(objectgroup, data)) return false;
		}
		else {
			g_logger->logError("LevelReader", "Objectgroup with unknown name found in level: " + name);
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
	size_t index = 0;
	std::string levelItem;
	while ((pos = layerData.find(",")) != std::string::npos) {
		id = std::stoi(layerData.substr(0, pos));
		layerData.erase(0, pos + 1);
		if (id == 0) {
			index++;
			continue;
		}

		id = id - m_firstGidItems;
		
		if (!contains(m_levelItemMap, id)) {
			logError("Level item ID not recognized: " + std::to_string(id));
			return false;
		}
		levelItem = m_levelItemMap.at(id);
		data.levelItems.at(index) = levelItem;
		index++;
	}

	id = std::stoi(layerData);
	if (id > 0) {
		id = id - m_firstGidItems;
		if (!contains(m_levelItemMap, id)) {
			logError("Level item ID not recognized: " + std::to_string(id));
			return false;
		}
		levelItem = m_levelItemMap.at(id);
		data.levelItems.at(index) = levelItem;
	}
	return true;
}

bool LevelReader::readDynamicTileLayer(const std::string& layer, LevelData& data) const {
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
			LevelDynamicTileID tileId = static_cast<LevelDynamicTileID>(id % DYNAMIC_TILE_COUNT + 1);
			int skinNr = id / DYNAMIC_TILE_COUNT;
			if (tileId <= LevelDynamicTileID::VOID || tileId >= LevelDynamicTileID::MAX) {
				logError("Unknown dynamic tile id found on dynamic layer: " + std::to_string(id));
				return false;
			}

			LevelDynamicTileData tileData;
			tileData.id = tileId;
			tileData.skinNr = skinNr;
			tileData.spawnPosition = position;

			data.dynamicTiles.push_back(tileData);
		}
		
		++position;
	}

	return true;
}

bool LevelReader::readFluidLayer(const std::string& layer, LevelData& data) const {
	std::string layerData = layer;
	int offset = static_cast<int>(LevelDynamicTileID::Fluid) + m_firstGidDynamicTiles - 1;
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

	// process layer

	std::vector<bool> processed(dynamicTileLayer.size(), false);
	LevelDynamicTileData ldtData;
	ldtData.id = LevelDynamicTileID::Fluid;

	for (int y = 0; y < data.mapSize.y; ++y) {
		for (int x = 0; x < data.mapSize.x; ++x) {
			int skinNr = dynamicTileLayer[y * data.mapSize.x + x];

			if (skinNr != 0 && !processed[y * data.mapSize.x + x]) {		// found start of unprocessed rectangle
				int x0 = x; int xi = x0;
				int y0 = y; int yi = y0;

				int width = 0;
				int height = 0;

				// Check size of rectangle in both x and y
				while (xi < data.mapSize.x && dynamicTileLayer[y0 * data.mapSize.x + xi] == skinNr && !processed[y0 * data.mapSize.x + xi]) {
					width++;
					xi++;
				}
				while (yi < data.mapSize.y && dynamicTileLayer[yi * data.mapSize.x + x0] == skinNr && !processed[yi * data.mapSize.x + x0]) {
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
				ldtData.skinNr = skinNr - 1;
				ldtData.spawnPosition = y * data.mapSize.x + x;
				ldtData.size = sf::Vector2f(width * TILE_SIZE_F, height * TILE_SIZE_F);
				data.dynamicTiles.push_back(ldtData);
			}
		}
	}

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
			lever.skinNr = ((skinNr - leverOffset) / DYNAMIC_TILE_COUNT);
			lever.spawnPosition = y * data.mapSize.x + x;
			leData.levers.push_back(lever);
		}
		else if ((skinNr - onOffset) % DYNAMIC_TILE_COUNT == 0) {
			LevelDynamicTileData switchTile;
			switchTile.id = LevelDynamicTileID::SwitchableOn;
			switchTile.position = sf::Vector2f(x * TILE_SIZE_F, y * TILE_SIZE_F);
			switchTile.skinNr = ((skinNr - onOffset) / DYNAMIC_TILE_COUNT);
			switchTile.spawnPosition = y * data.mapSize.x + x;
			leData.dependentSwitchableTiles.push_back(switchTile);
		}
		else if ((skinNr - offOffset) % DYNAMIC_TILE_COUNT == 0) {
			LevelDynamicTileData switchTile;
			switchTile.id = LevelDynamicTileID::SwitchableOff;
			switchTile.position = sf::Vector2f(x * TILE_SIZE_F, y * TILE_SIZE_F);
			switchTile.skinNr = ((skinNr - offOffset) / DYNAMIC_TILE_COUNT);
			switchTile.spawnPosition = y * data.mapSize.x + x;
			leData.dependentSwitchableTiles.push_back(switchTile);
		}
		else {
			logError("Wrong tile id found on a lever layer, id = " + std::to_string(skinNr));
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
	// pre-load level item layer
	data.levelItems.clear();
	for (int i = 0; i < data.mapSize.x * data.mapSize.y; ++i) {
		data.levelItems.push_back("");
	}

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
		else if (name.find("lever") != std::string::npos) {
			if (!readLeverLayer(layerData, data)) return false;
		}
		else if (name.find("dynamic") != std::string::npos) {
			if (!readDynamicTileLayer(layerData, data)) return false;
		}
		else if (name.find("fluid") != std::string::npos) {
			if (!readFluidLayer(layerData, data)) return false;
		}
		else if (name.find("collidable") != std::string::npos) {
			if (!readCollidableLayer(layerData, data)) return false;
		}
		else if (name.find("item") != std::string::npos) {
			if (!readLevelItemLayer(layerData, data)) return false;
		}
		else {
			g_logger->logError("LevelReader", "Layer with unknown name found in level: " + name);
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

bool LevelReader::readMapProperties(tinyxml2::XMLElement* map, WorldData& data_) const {
	if (!WorldReader::readMapProperties(map, data_)) return false;

	// read level properties
	tinyxml2::XMLElement* properties = map->FirstChildElement("properties");
	if (properties == nullptr) {
		logError("XML file could not be read, no properties node found.");
		return false;
	}
	tinyxml2::XMLElement* _property = properties->FirstChildElement("property");

	LevelData& data = static_cast<LevelData&>(data_);

	const char* textAttr = nullptr;
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
		else if (name.compare("weather") == 0) {
			if (!readWeather(_property, data)) return false;
		}
		else if (name.compare("observed") == 0) {
			data.isObserved = true;
		}
		else if (name.compare("bosslevel") == 0) {
			if (!readBosslevel(_property, data)) return false;
		}
		else {
			logError("XML file could not be read, unknown name attribute found in properties (map->properties->property->name).");
			return false;
		}

		_property = _property->NextSiblingElement("property");
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

bool LevelReader::readBosslevel(tinyxml2::XMLElement* _property, WorldData& data_) const {
	LevelData* data = static_cast<LevelData*>(&data_);
	if (data == nullptr) return false;
	const char* textAttr = nullptr;
	textAttr = _property->Attribute("value");
	if (textAttr == nullptr) {
		logError("XML file could not be read, no value attribute found (map->properties->property->name=bosslevel).");
		return false;
	}
	data->bossLevelPath = textAttr;
	data->isBossLevel = true;

	return true;
}

bool LevelReader::checkData(LevelData& data) const {
	if (!WorldReader::checkData(data)) return false;
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
	for (auto& lever : data.levers) {
		for (auto& it : lever.dependentMovingTiles) {
			if (it.initialDirection == -1) {
				logError("Moving tile initial direction must be set.");
				return false;
			}
			if (it.speed == 0.f) {
				g_logger->logWarning("LevelReader", "Moving tile speed is 0.");
			}
			if (it.distance == 0) {
				g_logger->logWarning("LevelReader", "Moving tile distance is 0.");
			}
		}
	}
	for (auto& it : data.enemies) {
		for (auto& target : it.questTargets) {
			if (target.first.empty() || target.second.empty()) {
				logError("enemy quest target quest and name must be filled.");
				return false;
			}
		}
	}
	if (static_cast<int>(data.levelItems.size()) != data.mapSize.x * data.mapSize.y) {
		logError("level item layer has not correct size (map size)");
		return false;
	}

	return true;
}