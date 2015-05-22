#include "CharacterCoreReader.h"

using namespace std;

CharacterCoreReader::CharacterCoreReader()
{
	initMaps();
}

CharacterCoreReader::~CharacterCoreReader()
{
	m_mapMap.clear();
	m_levelMap.clear();
	m_itemMap.clear();
}

void CharacterCoreReader::initMaps()
{
	m_mapMap.insert({1, MapID::Testmap});
	m_levelMap.insert({ 1, LevelID::Testlevel });
	m_itemMap.insert({ 
		{ 0, ItemID::Void },
		{ 1, ItemID::Equipment_head_wizardhat_blue },
		{ 2, ItemID::Equipment_head_wizardhat_black},
		{ 3, ItemID::Equipment_weapon_staff_ice },
		{ 4, ItemID::Food_Cheese },
		{ 5, ItemID::Food_Bread },
		{ 6, ItemID::Food_Water },
	});
}

bool CharacterCoreReader::checkData(CharacterCoreData& data) const
{
	if (data.playerName.empty())
	{
		g_logger->logError("CharacterCoreReader", "Error in savegame data : player name not set");
		return false;
	}
	if (data.currentMapPosition.x < 0 || data.currentMapPosition.y < 0)
	{
		g_logger->logError("CharacterCoreReader", "Error in savegame data : map position negative");
		return false;
	}
	if (data.currentMap == MapID::Void)
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
		|| data.attributes.maxManaPoints < 0
		|| data.attributes.healthRegenerationPerS < 0
		|| data.attributes.manaRegenerationPerS < 0
		|| data.attributes.damagePhysical < 0
		|| data.attributes.damageFire < 0
		|| data.attributes.damageIce < 0
		|| data.attributes.resistancePhysical < 0
		|| data.attributes.resistanceFire < 0
		|| data.attributes.resistanceIce < 0)
	{
		g_logger->logError("CharacterCoreReader", "Error in savegame data : attributes cannot be negative");
		return false;
	}
	for (auto &it : data.levelKilled)
	{
		if (it.first == LevelID::Void)
		{
			g_logger->logError("CharacterCoreReader", "Error in savegame data : level not resolved");
			return false;
		}
	}
	for (auto &it : data.levelLooted)
	{
		if (it.first == LevelID::Void)
		{
			g_logger->logError("CharacterCoreReader", "Error in savegame data : level not resolved");
			return false;
		}
	}
	
	return true;
}

bool CharacterCoreReader::readPlayerName(char* start, char* end, CharacterCoreData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	string name(startData);
	int count = countToNextChar(startData, end, '"');
	if (count == -1) {
		return false;
	}
	name = name.substr(0, count);
	data.playerName = name;
	return true;
}

bool CharacterCoreReader::readTimePlayed(char* start, char* end, CharacterCoreData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	int x = atoi(startData);
	if (x < 0)
	{
		return false;
	}
	data.timePlayed = sf::seconds(static_cast<float>(x));
	return true;
}

bool CharacterCoreReader::readGold(char* start, char* end, CharacterCoreData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	data.gold = atoi(startData);
	return true;
}

bool CharacterCoreReader::readMapID(char* start, char* end, CharacterCoreData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	int id = atoi(startData);
	if (m_mapMap.find(id) == m_mapMap.end())
	{
		g_logger->logError("CharacterCoreReader", "Map ID not recognized: " + id);
		return false;
	}
	data.currentMap = m_mapMap.at(id);
	return true;
}

bool CharacterCoreReader::readMapPosition(char* start, char* end, CharacterCoreData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	int x = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	int y = atoi(startData);
	data.currentMapPosition = sf::Vector2f(static_cast<float>(x), static_cast<float>(y));
	return true;
}

bool CharacterCoreReader::readAttributes(char* start, char* end, CharacterCoreData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	data.attributes.maxHealthPoints = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.maxManaPoints = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.healthRegenerationPerS = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.manaRegenerationPerS = atoi(startData);
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
	data.attributes.resistancePhysical = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.resistanceFire = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	data.attributes.resistancePhysical = atoi(startData);
	return true;
}

bool CharacterCoreReader::readItemID(char* start, char* end, CharacterCoreData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	int id = atoi(startData);
	if (id == 0 || m_itemMap.find(id) == m_itemMap.end())
	{
		g_logger->logError("CharacterCoreReader", "Item ID not recognized: " + id);
		return false;
	}
	ItemID item = m_itemMap.at(id);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	int amount = atoi(startData);
	data.items.insert({item, amount});
	return true;
}

bool CharacterCoreReader::readEquippedItem(char* start, char* end, CharacterCoreData& data, ItemType type) const
{
	char* startData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	int id = atoi(startData);
	if (m_itemMap.find(id) == m_itemMap.end())
	{
		g_logger->logError("CharacterCoreReader", "Item ID not recognized: " + id);
		return false;
	}
	ItemID item = m_itemMap.at(id);

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

bool CharacterCoreReader::readLevelKilled(char* start, char* end, CharacterCoreData& data) const
{
	// add a level state
	vector<bool> layer;

	char* startData;
	char* endData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	endData = gotoNextChar(startData, end, '"');

	int id = atoi(startData);
	if (m_levelMap.find(id) == m_levelMap.end())
	{
		g_logger->logError("CharacterCoreReader", "Level ID not recognized: " + id);
		return false;
	}
	LevelID level = m_levelMap.at(id);
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

	data.levelKilled.insert({level, layer});
	return true;
}

bool CharacterCoreReader::readLevelLooted(char* start, char* end, CharacterCoreData& data) const
{
	// add a level state
	vector<bool> layer;

	char* startData;
	char* endData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	endData = gotoNextChar(startData, end, '"');

	int id = atoi(startData);
	if (m_levelMap.find(id) == m_levelMap.end())
	{
		g_logger->logError("CharacterCoreReader", "Level ID not recognized: " + id);
		return false;
	}
	LevelID level = m_levelMap.at(id);
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

	data.levelLooted.insert({ level, layer });
	return true;
}

bool CharacterCoreReader::readCharacterCore(char* fileName, CharacterCoreData& data)
{
	FILE* savFile;
	savFile = fopen(fileName, "r");

	if (savFile == NULL)
	{
		g_logger->logError("CharacterCoreReader", "Error at opening file " + std::string(fileName));
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
		if (*pos == COMMENT_MARKER)
		{
			pos = gotoNextChar(pos, end, '\n');
		}

		else if (strncmp(pos, PLAYER_NAME, strlen(PLAYER_NAME)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(PLAYER_NAME));
			noError = readPlayerName(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, TIME_PLAYED, strlen(TIME_PLAYED)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(TIME_PLAYED));
			noError = readTimePlayed(pos, end, data);
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
		else if (strncmp(pos, EQUIPPED_WEAPON, strlen(EQUIPPED_WEAPON)) == 0) {
			g_logger->log(LogLevel::Verbose, "CharacterCoreReader", "found tag " + std::string(EQUIPPED_WEAPON));
			noError = readEquippedItem(pos, end, data, ItemType::Equipment_weapon);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, EQUIPPED_WEAPON, strlen(EQUIPPED_BODY)) == 0) {
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
		else {
			g_logger->logError("CharacterCoreReader", "unknown tag found in file " + std::string(fileName));
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

