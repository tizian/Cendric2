#include "FileIO/CharacterCoreReader.h"

#include <cstring>

using namespace std;

bool CharacterCoreReader::checkData(CharacterCoreData& data) const
{
	if (data.currentMapPosition.x < 0 || data.currentMapPosition.y < 0)
	{
		g_logger->logError("CharacterCoreReader", "Error in savegame data : map position negative");
		return false;
	}
	if (data.currentMap == MapID::VOID)
	{
		g_logger->logError("CharacterCoreReader", "Error in savegame data : map not resolved");
		return false;
	}
	if (data.gold < 0)
	{
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
		|| data.attributes.damageLight < 0)
	{
		g_logger->logError("CharacterCoreReader", "Error in savegame data : (some) attributes cannot be negative");
		return false;
	}
	for (auto &it : data.levelKilled)
	{
		if (it.first == LevelID::VOID)
		{
			g_logger->logError("CharacterCoreReader", "Error in savegame data : level not resolved");
			return false;
		}
	}
	for (auto &it : data.levelLooted)
	{
		if (it.first == LevelID::VOID)
		{
			g_logger->logError("CharacterCoreReader", "Error in savegame data : level not resolved");
			return false;
		}
	}
	if (data.equippedWeaponSlots.size() > 5)
	{
		g_logger->logError("CharacterCoreReader", "Error in savegame data : there can't be more spell slots than 5 on a weapon");
		return false;
	}
	
	return true;
}

bool CharacterCoreReader::readTimePlayed(char* start, char* end, CharacterCoreData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	int x = atoi(startData);
	if (x < 0)
	{
		return false;
	}
	data.timePlayed = sf::seconds(static_cast<float>(x));
	return true;
}

bool CharacterCoreReader::readSavegameName(char* start, char* end, CharacterCoreData& data) const
{
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

bool CharacterCoreReader::readQuickslot(char* start, char* end, CharacterCoreData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	int nr = atoi(startData);
	if (nr < 1 || nr > 2)
	{
		g_logger->logError("CharacterCoreReader::readQuickslot", "Quickslot nr not valid.");
		return false;
	}
	startData = gotoNextChar(startData, end, ',');
	startData++;
	string item(startData);
	int count = countToNextChar(startData, end, '\n');
	if (count == -1) 
	{
		return false;
	}
	item = item.substr(0, count);

	if (nr == 1)
	{
		data.quickSlot1 = item;
	}
	else
	{
		data.quickSlot2 = item;
	}
	return true;
}

bool CharacterCoreReader::readSavegameDate(char* start, char* end, CharacterCoreData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++; 
	data.dateSaved = (atol(startData));
	return true;
}

bool CharacterCoreReader::readGold(char* start, char* end, CharacterCoreData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	data.gold = atoi(startData);
	return true;
}

bool CharacterCoreReader::readMapID(char* start, char* end, CharacterCoreData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	MapID id = static_cast<MapID>(atoi(startData));
	if (id <= MapID::VOID || id >= MapID::MAX)
	{
		g_logger->logError("CharacterCoreReader", "Map ID not recognized: " + std::to_string(static_cast<int>(id)));
		return false;
	}
	data.currentMap = id;
	return true;
}

bool CharacterCoreReader::readMapPosition(char* start, char* end, CharacterCoreData& data) const
{
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

bool CharacterCoreReader::readLevelID(char* start, char* end, CharacterCoreData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	LevelID id = static_cast<LevelID>(atoi(startData));
	if (id < LevelID::VOID || id >= LevelID::MAX)
	{
		g_logger->logError("CharacterCoreReader", "Level ID not recognized: " + std::to_string(static_cast<int>(id)));
		return false;
	}
	data.currentLevel = id;
	return true;
}

bool CharacterCoreReader::readLevelPosition(char* start, char* end, CharacterCoreData& data) const
{
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

bool CharacterCoreReader::readAttributes(char* start, char* end, CharacterCoreData& data) const
{
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

bool CharacterCoreReader::readItemID(char* start, char* end, CharacterCoreData& data) const
{
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
	data.items.insert({id, amount});
	return true;
}

bool CharacterCoreReader::readQuestStates(char* start, char* end, CharacterCoreData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	QuestID id = static_cast<QuestID>(atoi(startData));
	if (id <= QuestID::Void || id >= QuestID::MAX)
	{
		g_logger->logError("CharacterCoreReader", "Quest ID not recognized: " + std::to_string(static_cast<int>(id)));
		return false;
	}
	startData = gotoNextChar(startData, end, ',');
	startData++;
	QuestState state = static_cast<QuestState>(atoi(startData));
	if (state <= QuestState::Void || state >= QuestState::MAX)
	{
		g_logger->logError("CharacterCoreReader", "Quest State not recognized: " + std::to_string(static_cast<int>(state)));
		return false;
	}
	data.questStates.insert({ id, state });
	return true;
}

bool CharacterCoreReader::readNPCStates(char* start, char* end, CharacterCoreData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	NPCID id = static_cast<NPCID>(atoi(startData));
	if (id <= NPCID::Void || id >= NPCID::MAX)
	{
		g_logger->logError("CharacterCoreReader", "NPC ID not recognized: " + std::to_string(static_cast<int>(id)));
		return false;
	}
	startData = gotoNextChar(startData, end, ',');
	startData++;
	NPCState state = static_cast<NPCState>(atoi(startData));
	if (state <= NPCState::Void || state >= NPCState::MAX)
	{
		g_logger->logError("CharacterCoreReader", "NPC State not recognized: " + std::to_string(static_cast<int>(state)));
		return false;
	}
	data.npcStates.insert({ id, state });
	return true;
}

bool CharacterCoreReader::readEquippedItem(char* start, char* end, CharacterCoreData& data, ItemType type) const
{
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	string item(startData);
	int count = countToNextChar(startData, end, '\n');
	if (count == -1) {
		return false;
	}
	item = item.substr(0, count);
	
	switch (type)
	{
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

bool CharacterCoreReader::readEquippedWeaponSlots(char* start, char* end, CharacterCoreData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;

	SpellID spell = static_cast<SpellID>(atoi(startData));
	if (spell <= SpellID::VOID || spell >= SpellID::MAX)
	{
		g_logger->logError("CharacterCoreReader", "Spell ID not recognized: " + to_string(static_cast<int>(spell)));
		return false;
	}

	char* endData = gotoNextChar(startData, end, '\n');
	if (endData == nullptr) return false;

	vector<SpellModifier> modifiers;
	
	startData = gotoNextChar(startData, end, ',');
	while (startData != nullptr && startData < endData)
	{
		startData++;
		SpellModifierType type = static_cast<SpellModifierType>(atoi(startData));
		if (type <= SpellModifierType::VOID || type >= SpellModifierType::MAX)
		{
			g_logger->logError("CharacterCoreReader", "Spell Modifier type not recognized: " + to_string(static_cast<int>(type)));
			return false;
		}
		startData = gotoNextChar(startData, end, ',');
		startData++;
		int level = atoi(startData);
		if (level < 1 || level > 3)
		{
			g_logger->logError("CharacterCoreReader", "Spell Modifier level is not allowed: " + to_string(level));
			return false;
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

bool CharacterCoreReader::readLevelKilled(char* start, char* end, CharacterCoreData& data) const
{
	// add a level state
	vector<bool> layer;

	char* startData;
	char* endData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	endData = gotoNextChar(startData, end, '\n');

	LevelID id = static_cast<LevelID>(atoi(startData));
	if (id <= LevelID::VOID || id >= LevelID::MAX)
	{
		g_logger->logError("CharacterCoreReader", "Level ID not recognized: " + std::to_string(static_cast<int>(id)));
		return false;
	}
	startData++;

	while (startData != NULL)
	{
		layer.push_back(atoi(startData) != 0);
		startData = gotoNextChar(startData, endData, ',');
		if (startData != NULL)
		{
			startData++;
		}
	}

	data.levelKilled.insert({id, layer});
	return true;
}

bool CharacterCoreReader::readLevelLooted(char* start, char* end, CharacterCoreData& data) const
{
	// add a level state
	vector<bool> layer;

	char* startData;
	char* endData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	endData = gotoNextChar(startData, end, '\n');

	LevelID id = static_cast<LevelID>(atoi(startData));
	if (id <= LevelID::VOID || id >= LevelID::MAX)
	{
		g_logger->logError("CharacterCoreReader", "Level ID not recognized: " + std::to_string(static_cast<int>(id)));
		return false;
	}
	startData++;

	while (startData != NULL)
	{
		layer.push_back(atoi(startData) != 0);
		startData = gotoNextChar(startData, endData, ',');
		if (startData != NULL)
		{
			startData++;
		}
	}

	data.levelLooted.insert({ id, layer });
	return true;
}

bool CharacterCoreReader::readCharacterCore(const std::string& filename, CharacterCoreData& data)
{
	FILE* savFile;
	savFile = fopen(filename.c_str(), "r");

	if (savFile == NULL)
	{
		g_logger->logError("CharacterCoreReader", "Error at opening file " + filename);
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
	while (pos < end)
	{
		if (*pos == COMMENT_MARKER || *pos == '\n')
		{
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
		else if (strncmp(pos, LEVEL_KILLED, strlen(LEVEL_KILLED)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(LEVEL_KILLED));
			noError = readLevelKilled(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LEVEL_LOOTED, strlen(LEVEL_LOOTED)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(LEVEL_LOOTED));
			noError = readLevelLooted(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, QUEST_STATE, strlen(QUEST_STATE)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(QUEST_STATE));
			noError = readQuestStates(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, NPC_STATE, strlen(NPC_STATE)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(NPC_STATE));
			noError = readNPCStates(pos, end, data);
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
		else {
			g_logger->logError("CharacterCoreReader", "unknown tag found in file " + filename);
			return false;
		}

		if (pos == NULL || !noError)
		{
			// reached end of file or error happened
			break;
		}

		pos++;
	}

	std::fclose(savFile);
	delete[] charBuffer;

	// check data
	if (!noError || !checkData(data))
	{
		return false;
	}

	return true;
}

