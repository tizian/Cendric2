#include "FileIO/CharacterCoreReader.h"

#include <cstring>

using namespace std;

bool CharacterCoreReader::checkData(CharacterCoreData& data) const {
	if (data.currentMapPosition.x < 0 || data.currentMapPosition.y < 0) {
		g_logger->logError("CharacterCoreReader", "Error in savegame data : map position negative");
		return false;
	}
	if (data.currentMap.empty()) {
		g_logger->logError("CharacterCoreReader", "Error in savegame data : map not resolved");
		return false;
	}
	if (data.gold < 0) {
		g_logger->logError("CharacterCoreReader", "Error in savegame data : gold cannot be negative");
		return false;
	}
	if (data.attributes.maxHealthPoints < 0
		|| data.attributes.healthRegenerationPerS < 0
		|| data.attributes.haste < 0
		|| data.attributes.critical < 0
		|| data.attributes.damagePhysical < 0
		|| data.attributes.damageFire < 0
		|| data.attributes.damageIce < 0
		|| data.attributes.damageShadow < 0
		|| data.attributes.damageLight < 0) {
		g_logger->logError("CharacterCoreReader", "Error in savegame data : (some) attributes cannot be negative");
		return false;
	}
	if (data.equippedWeaponSlots.size() > 5) {
		g_logger->logError("CharacterCoreReader", "Error in savegame data : there can't be more spell slots than 5 on a weapon");
		return false;
	}
	for (auto& it : data.questStates) {
		if (it.first.empty()) {
			g_logger->logError("CharacterCoreReader", "Error in savegame data : quest id empty");
			return false;
		}
	}

	return true;
}

bool CharacterCoreReader::readTimePlayed(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	int x = atoi(startData);
	if (x < 0) {
		return false;
	}
	data.timePlayed = sf::seconds(static_cast<float>(x));
	return true;
}

bool CharacterCoreReader::readSavegameName(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	string name(startData);
	int count = countToNextChar(startData, end, '\n');
	if (count == -1) {
		return false;
	}
	name = name.substr(0, count);
	data.saveGameName = name;
	return true;
}

bool CharacterCoreReader::readQuickslot(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	int nr = atoi(startData);
	if (nr < 1 || nr > 2) {
		g_logger->logError("CharacterCoreReader::readQuickslot", "Quickslot nr not valid.");
		return false;
	}
	startData = gotoNextChar(startData, end, ',');
	startData++;
	string item(startData);
	int count = countToNextChar(startData, end, '\n');
	if (count == -1) {
		return false;
	}
	item = item.substr(0, count);

	if (nr == 1) {
		data.quickSlot1 = item;
	}
	else {
		data.quickSlot2 = item;
	}
	return true;
}

bool CharacterCoreReader::readSavegameDate(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	data.dateSaved = (atol(startData));
	return true;
}

bool CharacterCoreReader::readGold(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	data.gold = atoi(startData);
	return true;
}

bool CharacterCoreReader::readMapID(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	string id(startData);
	int count = countToNextChar(startData, end, '\n');
	if (count == -1) {
		return false;
	}
	id = id.substr(0, count);
	data.currentMap = id;
	return true;
}

bool CharacterCoreReader::readMapPosition(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	float x = static_cast<float>(atof(startData));
	startData = gotoNextChar(startData, end, ',');
	startData++;
	float y = static_cast<float>(atof(startData));
	data.currentMapPosition = sf::Vector2f(x, y);
	return true;
}

bool CharacterCoreReader::readLevelID(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	string id(startData);
	int count = countToNextChar(startData, end, '\n');
	if (count == -1) {
		return false;
	}
	id = id.substr(0, count);
	data.currentLevel = id;
	return true;
}

bool CharacterCoreReader::readLevelPosition(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	float x = static_cast<float>(atof(startData));
	startData = gotoNextChar(startData, end, ',');
	startData++;
	float y = static_cast<float>(atof(startData));
	data.currentLevelPosition = sf::Vector2f(x, y);
	return true;
}

bool CharacterCoreReader::readAttributes(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	data.attributes.maxHealthPoints = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.healthRegenerationPerS = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.haste = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.critical = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.damagePhysical = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.damageFire = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.damageIce = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.damageShadow = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.damageLight = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.resistancePhysical = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.resistanceFire = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.resistanceIce = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.resistanceShadow = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.resistanceLight = atoi(startData);
	return true;
}

bool CharacterCoreReader::readItemID(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	string id(startData);
	int count = countToNextChar(startData, end, ',');
	if (count == -1) {
		return false;
	}
	id = id.substr(0, count);

	startData = gotoNextChar(startData, end, ',');
	startData++;
	int amount = atoi(startData);
	data.items.insert({ id, amount });
	return true;
}

bool CharacterCoreReader::readIsInLevel(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	data.isInLevel = atoi(startData) == 1;
	return true;
}

bool CharacterCoreReader::readMerchandState(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	char* endData = gotoNextChar(start, end, '\n');
	endData++;
	startData = gotoNextChar(start, endData, ':');
	startData++;

	string id(startData);
	int count = countToNextChar(startData, end, ',');
	if (count == -1) {
		return false;
	}
	id = id.substr(0, count);

	startData = gotoNextChar(start, endData, ',');
	startData++;

	FractionID fraction = (FractionID)(atoi(startData));
	if (fraction < FractionID::VOID || fraction >= FractionID::MAX) {
		g_logger->logError("CharacterCoreReader", "FractionID not resolved " + to_string((int)fraction));
		return false;
	}

	startData = gotoNextChar(startData, endData, ',');
	startData++;

	float multiplier = (float)(atof(startData));
	if (multiplier < 1.f) {
		g_logger->logError("CharacterCoreReader", "Multiplier value not allowed " + to_string(multiplier));
		return false;
	}

	startData = gotoNextChar(startData, endData, ',');

	std::map<std::string, int> wares;
	while (startData != NULL) {
		startData++;
		std::string name(startData);
		count = countToNextChar(startData, endData, ',');
		if (count == -1) {
			return false;
		}
		name = name.substr(0, count);

		startData = gotoNextChar(startData, endData, ',');
		startData++;

		int amount = atoi(startData);
		if (amount <= 0) {
			return false;
		}

		wares.insert({ name, amount });
		startData = gotoNextChar(startData, endData, ',');
	}

	MerchantData merchandData;
	merchandData.wares = wares;
	merchandData.fraction = fraction;
	merchandData.multiplier = multiplier;

	data.merchantStates.insert({ id, merchandData });
	return true;
}

bool CharacterCoreReader::readQuestStates(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	string questID(startData);
	int count = countToNextChar(startData, end, ',');
	if (count == -1) {
		return false;
	}
	questID = questID.substr(0, count);

	startData = gotoNextChar(startData, end, ',');
	startData++;
	QuestState state = static_cast<QuestState>(atoi(startData));
	if (state <= QuestState::VOID || state >= QuestState::MAX) {
		g_logger->logError("CharacterCoreReader", "Quest State not recognized: " + std::to_string(static_cast<int>(state)));
		return false;
	}
	data.questStates.insert({ questID, state });
	return true;
}

bool CharacterCoreReader::readQuestProgressConditions(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	char* endData = gotoNextChar(start, end, '\n');
	endData++;
	startData = gotoNextChar(start, end, ':');
	startData++;
	string questID(startData);
	int count = countToNextChar(startData, endData, ',');
	if (count == -1) {
		return false;
	}
	questID = questID.substr(0, count);

	startData = gotoNextChar(startData, endData, ',');
	startData++;

	std::set<string> conditions;
	while (startData != NULL) {
		std::string condition(startData);
		count = countToNextChar(startData, endData, ',');
		if (count == -1) {
			count = countToNextChar(startData, endData, '\n');
			if (count == -1) {
				return false;
			}
			conditions.insert(condition.substr(0, count));
			break;
		}
		conditions.insert(condition.substr(0, count));
		startData = gotoNextChar(startData, endData, ',');
		startData++;
	}

	data.questConditionProgress.insert({ questID, conditions });
	return true;
}

bool CharacterCoreReader::readQuestProgressDescription(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	char* endData = gotoNextChar(start, end, '\n');
	endData++;
	startData = gotoNextChar(start, end, ':');
	startData++;
	string questID(startData);
	int count = countToNextChar(startData, endData, ',');
	if (count == -1) {
		return false;
	}
	questID = questID.substr(0, count);

	startData = gotoNextChar(startData, endData, ',');
	startData++;

	std::set<int> descriptions;
	while (startData != NULL) {
		std::string description(startData);
		count = countToNextChar(startData, endData, ',');
		if (count == -1) {
			count = countToNextChar(startData, endData, '\n');
			if (count == -1) {
				return false;
			}

			description = description.substr(0, count);
			int descriptionID = atoi(description.c_str());
			if (descriptionID < 1) {
				g_logger->logError("CharacterCoreReader", "Cannot insert quest description id:" + description);
				return false;
			}
			descriptions.insert(descriptionID);
			break;
		}
		description = description.substr(0, count);
		int descriptionID = atoi(description.c_str());
		if (descriptionID < 1) {
			g_logger->logError("CharacterCoreReader", "Cannot insert quest description id:" + description);
			return false;
		}
		descriptions.insert(descriptionID);
		startData = gotoNextChar(startData, endData, ',');
		startData++;
	}

	data.questDescriptionProgress.insert({ questID, descriptions });
	return true;
}

bool CharacterCoreReader::readProgressConditions(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	char* endData = gotoNextChar(start, end, '\n');
	endData++;
	startData = gotoNextChar(start, end, ':');
	startData++;
	string progressType(startData);
	int count = countToNextChar(startData, endData, ',');
	if (count == -1) {
		return false;
	}
	progressType = progressType.substr(0, count);

	if (data.conditionProgress.find(progressType) != data.conditionProgress.end()) {
		g_logger->logError("CharacterCoreReader", "Duplicate condition progress type encountered: " + progressType);
		return false;
	}

	startData = gotoNextChar(startData, endData, ',');
	
	std::set<string> conditions;
	while (startData != NULL) {
		std::string condition(startData);
		int count = countToNextChar(startData, endData, ',');
		if (count == -1) {
			count = countToNextChar(startData, endData, '\n');
			if (count == -1) {
				return false;
			}
			conditions.insert(condition.substr(0, count));
			break;
		}
		conditions.insert(condition.substr(0, count));
		startData = gotoNextChar(startData, endData, ',');
		startData++;
	}

	data.conditionProgress.insert({ progressType, conditions });
	return true;
}

bool CharacterCoreReader::readQuestProgressTargets(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	char* endData = gotoNextChar(start, end, '\n');
	endData++;
	startData = gotoNextChar(start, end, ':');
	startData++;
	string questID(startData);
	int count = countToNextChar(startData, endData, ',');
	if (count == -1) {
		return false;
	}
	questID = questID.substr(0, count);

	startData = gotoNextChar(startData, endData, ',');

	std::map<std::string, int> targets;
	while (startData != NULL) {
		startData++;
		std::string name(startData);
		count = countToNextChar(startData, endData, ',');
		if (count == -1) {
			return false;
		}
		name = name.substr(0, count);

		startData = gotoNextChar(startData, endData, ',');
		startData++;

		int amount = atoi(startData);
		if (amount == 0) {
			return false;
		}

		targets.insert({ name, amount });
		startData = gotoNextChar(startData, endData, ',');
	}

	data.questTargetProgress.insert({ questID, targets });
	return true;
}

bool CharacterCoreReader::readLearnedSpells(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	char* endData = gotoNextChar(start, end, '\n');
	startData = gotoNextChar(start, endData, ':');
	startData++;

	SpellType type = static_cast<SpellType>(atoi(startData));
	if (type <= SpellType::VOID || type >= SpellType::MAX) {
		g_logger->logError("CharacterCoreReader", "Spell type not recognized: " + std::to_string(static_cast<int>(type)));
		return false;
	}

	startData = gotoNextChar(startData, endData, ',');

	std::set<SpellID> learnedSpells;
	while (startData != NULL) {
		startData++;
		SpellID id = static_cast<SpellID>(atoi(startData));
		if (id <= SpellID::VOID || id >= SpellID::MAX) {
			g_logger->logError("CharacterCoreReader", "Spell id not recognized: " + std::to_string(static_cast<int>(id)));
			return false;
		}
		learnedSpells.insert(id);
		startData = gotoNextChar(startData, endData, ',');
	}

	data.spellsLearned.insert({ type, learnedSpells });
	return true;
}

bool CharacterCoreReader::readLearnedModifiers(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;

	SpellModifierType type = static_cast<SpellModifierType>(atoi(startData));
	if (type <= SpellModifierType::VOID || type >= SpellModifierType::MAX) {
		g_logger->logError("CharacterCoreReader", "Spell modifier type not recognized: " + std::to_string(static_cast<int>(type)));
		return false;
	}

	startData = gotoNextChar(startData, end, ',');
	startData++;
	int level = atoi(startData);
	if (level < 1 || level > 3) {
		g_logger->logError("CharacterCoreReader", "Spell modifier type level is not between 1 and 3");
		return false;
	}

	data.modfiersLearned.insert({ type, level });
	return true;
}

bool CharacterCoreReader::readEquippedItem(char* start, char* end, CharacterCoreData& data, ItemType type) const {
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	string item(startData);
	int count = countToNextChar(startData, end, '\n');
	if (count == -1) {
		return false;
	}
	item = item.substr(0, count);

	switch (type) {
	case ItemType::Equipment_head:
		data.equippedHead = item;
		break;
	case ItemType::Equipment_ring_1:
		data.equippedRing1 = item;
		break;
	case ItemType::Equipment_ring_2:
		data.equippedRing2 = item;
		break;
	case ItemType::Equipment_weapon:
		data.equippedWeapon = item;
		break;
	case ItemType::Equipment_body:
		data.equippedBody = item;
		break;
	case ItemType::Equipment_neck:
		data.equippedNeck = item;
		break;
	case ItemType::Equipment_back:
		data.equippedBack = item;
		break;
	default:
		return false;
	}
	return true;
}

bool CharacterCoreReader::readEquippedWeaponSlots(char* start, char* end, CharacterCoreData& data) const {
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;

	SpellID spell = static_cast<SpellID>(atoi(startData));
	if (spell < SpellID::VOID || spell >= SpellID::MAX) {
		g_logger->logError("CharacterCoreReader", "Spell ID not recognized: " + to_string(static_cast<int>(spell)));
		return false;
	}

	char* endData = gotoNextChar(startData, end, '\n');
	if (endData == nullptr) return false;

	vector<SpellModifier> modifiers;

	startData = gotoNextChar(startData, end, ',');
	while (startData != nullptr && startData < endData) {
		startData++;
		SpellModifierType type = static_cast<SpellModifierType>(atoi(startData));
		if (type < SpellModifierType::VOID || type >= SpellModifierType::MAX) {
			g_logger->logError("CharacterCoreReader", "Spell Modifier type not recognized: " + to_string(static_cast<int>(type)));
			return false;
		}
		startData = gotoNextChar(startData, end, ',');
		startData++;
		int level = atoi(startData);
		if (level < 1 || level > 3) {
			if (type == SpellModifierType::VOID) {
				level = 0;
			}
			else {
				g_logger->logError("CharacterCoreReader", "Spell Modifier level is not allowed: " + to_string(level));
				return false;
			}
		}
		SpellModifier modifier;
		modifier.type = type;
		modifier.level = level;
		modifiers.push_back(modifier);

		startData = gotoNextChar(startData, end, ',');
	}

	data.equippedWeaponSlots.push_back(std::pair<SpellID, std::vector<SpellModifier>>(spell, modifiers));

	return true;
}

bool CharacterCoreReader::readLevelStateLayer(char* start, char* end, set<int>& layer, std::string& id) const {
	char* startData;
	char* endData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	endData = gotoNextChar(startData, end, '\n');
	endData++;
	string levelID(startData);
	int count = countToNextChar(startData, endData, ',');
	int count2 = countToNextChar(startData, endData, '\n');
	if (count == -1 && count2 == -1) {
		return false;
	}

	if (count != -1) {
		id = levelID.substr(0, count);
		startData = gotoNextChar(start, endData, ',');
		startData++;

		while (startData != NULL) {
			layer.insert(atoi(startData));
			startData = gotoNextChar(startData, endData, ',');
			if (startData != NULL) {
				startData++;
			}
		}
	}
	else {
		id = levelID.substr(0, count2);
		layer.clear();
	}
	return true;
}

bool CharacterCoreReader::readEnemiesKilled(char* start, char* end, CharacterCoreData& data) const {
	set<int> layer;
	std::string id;
	if (!readLevelStateLayer(start, end, layer, id)) return false;
	data.enemiesKilled.insert({ id, layer });
	return true;
}

bool CharacterCoreReader::readEnemiesLooted(char* start, char* end, CharacterCoreData& data) const {
	set<int> layer;
	std::string id;
	if (!readLevelStateLayer(start, end, layer, id)) return false;
	data.enemiesLooted.insert({ id, layer });
	return true;
}

bool CharacterCoreReader::readItemsLooted(char* start, char* end, CharacterCoreData& data) const {
	set<int> layer;
	std::string id;
	if (!readLevelStateLayer(start, end, layer, id)) return false;
	data.itemsLooted.insert({ id, layer });
	return true;
}

bool CharacterCoreReader::readChestsLooted(char* start, char* end, CharacterCoreData& data) const {
	set<int> layer;
	std::string id;
	if (!readLevelStateLayer(start, end, layer, id)) return false;
	data.chestsLooted.insert({ id, layer });
	return true;
}

bool CharacterCoreReader::readWaypointsUnlocked(char* start, char* end, CharacterCoreData& data) const {
	set<int> layer;
	std::string id;
	if (!readLevelStateLayer(start, end, layer, id)) return false;
	data.waypointsUnlocked.insert({ id, layer });
	return true;
}

bool CharacterCoreReader::readCharacterCore(const std::string& filename, CharacterCoreData& data, bool onlySaveGame) {
	FILE* savFile;
	savFile = fopen(getPath(filename).c_str(), "r");

	if (savFile == NULL) {
		g_logger->logError("CharacterCoreReader", "Error at opening file " + getPath(filename));
		return false;
	}

	// obtain file size
	long fileSize;
	long returnedFileSize;

	fseek(savFile, 0, SEEK_END);
	fileSize = ftell(savFile);
	rewind(savFile);

	// huge buffer array
	char* charBuffer = new char[fileSize];

	// fill buffer array
	returnedFileSize = static_cast<long>(fread(charBuffer, sizeof(char), fileSize, savFile));

	long bufferContentLength = returnedFileSize;

	char* pos = charBuffer;
	char* end = charBuffer + bufferContentLength;

	bool noError = true;

	// read defined tags
	while (pos < end) {
		if (*pos == COMMENT_MARKER || *pos == '\n') {
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, TIME_PLAYED, strlen(TIME_PLAYED)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(TIME_PLAYED));
			noError = readTimePlayed(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, SAVE_GAME_NAME, strlen(SAVE_GAME_NAME)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(SAVE_GAME_NAME));
			noError = readSavegameName(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, DATE_SAVED, strlen(DATE_SAVED)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(DATE_SAVED));
			noError = readSavegameDate(pos, end, data);
			if (onlySaveGame) {
				std::fclose(savFile);
				delete[] charBuffer;
				return noError;
			}
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, ATTRIBUTES, strlen(ATTRIBUTES)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(ATTRIBUTES));
			noError = readAttributes(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, MAP_ID, strlen(MAP_ID)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(MAP_ID));
			noError = readMapID(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, MAP_POSITION, strlen(MAP_POSITION)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(MAP_POSITION));
			noError = readMapPosition(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LEVEL_ID, strlen(LEVEL_ID)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(LEVEL_ID));
			noError = readLevelID(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LEVEL_POSITION, strlen(LEVEL_POSITION)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(LEVEL_POSITION));
			noError = readLevelPosition(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, ENEMIES_KILLED, strlen(ENEMIES_KILLED)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(ENEMIES_KILLED));
			noError = readEnemiesKilled(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, ENEMIES_LOOTED, strlen(ENEMIES_LOOTED)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(ENEMIES_LOOTED));
			noError = readEnemiesLooted(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, ITEMS_LOOTED, strlen(ITEMS_LOOTED)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(ITEMS_LOOTED));
			noError = readItemsLooted(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, CHESTS_LOOTED, strlen(CHESTS_LOOTED)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(CHESTS_LOOTED));
			noError = readChestsLooted(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, WAYPOINTS_UNLOCKED, strlen(WAYPOINTS_UNLOCKED)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(WAYPOINTS_UNLOCKED));
			noError = readWaypointsUnlocked(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, QUEST_STATE, strlen(QUEST_STATE)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(QUEST_STATE));
			noError = readQuestStates(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, QUEST_PROGRESS_TARGET, strlen(QUEST_PROGRESS_TARGET)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(QUEST_PROGRESS_TARGET));
			noError = readQuestProgressTargets(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, QUEST_PROGRESS_CONDITION, strlen(QUEST_PROGRESS_CONDITION)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(QUEST_PROGRESS_CONDITION));
			noError = readQuestProgressConditions(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, QUEST_PROGRESS_DESCRIPTION, strlen(QUEST_PROGRESS_DESCRIPTION)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(QUEST_PROGRESS_DESCRIPTION));
			noError = readQuestProgressDescription(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, PROGRESS_CONDITION, strlen(PROGRESS_CONDITION)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(PROGRESS_CONDITION));
			noError = readProgressConditions(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, GOLD, strlen(GOLD)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(GOLD));
			noError = readGold(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, ITEM_ID, strlen(ITEM_ID)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(ITEM_ID));
			noError = readItemID(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, EQUIPPED_SPELLSLOT, strlen(EQUIPPED_SPELLSLOT)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(EQUIPPED_SPELLSLOT));
			noError = readEquippedWeaponSlots(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, EQUIPPED_WEAPON, strlen(EQUIPPED_WEAPON)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(EQUIPPED_WEAPON));
			noError = readEquippedItem(pos, end, data, ItemType::Equipment_weapon);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, EQUIPPED_BODY, strlen(EQUIPPED_BODY)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(EQUIPPED_BODY));
			noError = readEquippedItem(pos, end, data, ItemType::Equipment_body);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, EQUIPPED_RING_1, strlen(EQUIPPED_RING_1)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(EQUIPPED_RING_1));
			noError = readEquippedItem(pos, end, data, ItemType::Equipment_ring_1);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, EQUIPPED_RING_2, strlen(EQUIPPED_RING_2)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(EQUIPPED_RING_2));
			noError = readEquippedItem(pos, end, data, ItemType::Equipment_ring_2);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, EQUIPPED_NECK, strlen(EQUIPPED_NECK)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(EQUIPPED_NECK));
			noError = readEquippedItem(pos, end, data, ItemType::Equipment_neck);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, EQUIPPED_BACK, strlen(EQUIPPED_BACK)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(EQUIPPED_WEAPON));
			noError = readEquippedItem(pos, end, data, ItemType::Equipment_back);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, EQUIPPED_HEAD, strlen(EQUIPPED_HEAD)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(EQUIPPED_HEAD));
			noError = readEquippedItem(pos, end, data, ItemType::Equipment_head);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, QUICKSLOT, strlen(QUICKSLOT)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(QUICKSLOT));
			noError = readQuickslot(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, IS_IN_LEVEL, strlen(IS_IN_LEVEL)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(IS_IN_LEVEL));
			noError = readIsInLevel(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, MERCHANT_STATE, strlen(MERCHANT_STATE)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(MERCHANT_STATE));
			noError = readMerchandState(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, SPELL_LEARNED, strlen(SPELL_LEARNED)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(SPELL_LEARNED));
			noError = readLearnedSpells(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, MODIFIER_LEARNED, strlen(MODIFIER_LEARNED)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(MODIFIER_LEARNED));
			noError = readLearnedModifiers(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else {
			g_logger->logError("CharacterCoreReader", "unknown tag found in file " + filename);
			return false;
		}

		if (pos == NULL || !noError) {
			// reached end of file or error happened
			break;
		}

		pos++;
	}

	std::fclose(savFile);
	delete[] charBuffer;

	// check data
	if (!noError || !checkData(data)) {
		return false;
	}

	return true;
}

