#include "FileIO/MapReader.h"

#include <cstring>

using namespace std;

MapReader::MapReader()
{
	initMaps();
}

MapReader::~MapReader()
{
	m_npcMap.clear();
}

void MapReader::initMaps()
{
	m_npcMap.insert({
		{ 0, NPCID::Void },
		{ 1, NPCID::Guard },
	});
}

bool MapReader::checkData(MapData& data) const
{
	if (data.mapSize.x == 0 || data.mapSize.y == 0)
	{
		g_logger->logError("MapReader", "Error in map data : map size not set / invalid");
		return false;
	}
	if (data.tileSize.x == 0 || data.tileSize.y == 0)
	{
		g_logger->logError("MapReader", "Error in map data : tile size not set / invalid");
		return false;
	}
	if (data.name.empty())
	{
		g_logger->logError("MapReader", "Error in map data : map name not set / empty");
		return false;
	}
	if (data.tileSetPath.empty())
	{
		g_logger->logError("MapReader", "Error in map data : tileset path not set / empty");
		return false;
	}
	if (data.backgroundLayers.empty())
	{
		g_logger->logError("MapReader", "Error in map data : no background layers set");
		return false;
	}
	for (int i = 0; i < data.backgroundLayers.size(); i++)
	{
		if (data.backgroundLayers[i].empty())
		{
			g_logger->logError("MapReader", "Error in map data : background layer " + std::to_string(i) + std::string(" empty"));
			return false;
		}
		if (data.backgroundLayers[i].size() != data.mapSize.x * data.mapSize.y)
		{
			g_logger->logError("MapReader", "Error in map data : background layer " + std::to_string(i) + std::string(" has not correct size (map size)"));
			return false;
		}
	}
	for (int i = 0; i < data.foregroundLayers.size(); i++)
	{
		if (data.foregroundLayers[i].empty())
		{
			g_logger->logError("MapReader", "Error in map data : foreground layer " + std::to_string(i) + std::string(" empty"));
			return false;
		}
		if (data.foregroundLayers[i].size() != data.mapSize.x * data.mapSize.y)
		{
			g_logger->logError("MapReader", "Error in map data : foreground layer " + std::to_string(i) + std::string(" has not correct size (map size)"));
			return false;
		}
	}
	for (auto it : data.levelEntries)
	{
		if (it.mapExitRect.left < 0.0 || it.mapExitRect.top < 0.0 || it.mapExitRect.left >= data.mapSize.x || it.mapExitRect.top >= data.mapSize.y)
		{
			g_logger->logError("MapReader", "Error in map data : a level entry rect is out of range for this map.");
			return false;
		}
	}
	if (data.collidableTiles.empty())
	{
		g_logger->logError("MapReader", "Error in map data : collidable layer is empty");
		return false;
	}
	if (data.collidableTiles.size() != data.mapSize.x * data.mapSize.y)
	{
		g_logger->logError("MapReader", "Error in map data : collidable layer has not correct size (map size)");
		return false;
	}

	return true;
}

bool MapReader::readMapName(char* start, char* end, MapData& data) const
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
	data.name = name;
	return true;
}

bool MapReader::readTilesetPath(char* start, char* end, MapData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	string path(startData);
	int count = countToNextChar(startData, end, '\n');
	if (count == -1) {
		return false;
	}
	path = path.substr(0, count);
	data.tileSetPath = path;
	return true;
}

bool MapReader::readLevelEntry(char* start, char* end, MapData& data) const
{
	char* startData;

	// read level entry rect
	startData = gotoNextChar(start, end, ':');
	startData++;
	float left = static_cast<float>(atof(startData));
	startData = gotoNextChar(startData, end, ',');
	startData++;
	float top = static_cast<float>(atof(startData));
	startData = gotoNextChar(startData, end, ',');
	startData++;
	float width = static_cast<float>(atof(startData));
	startData = gotoNextChar(startData, end, ',');
	startData++;
	float height = static_cast<float>(atof(startData));
	sf::FloatRect entry(left, top, width, height);

	// read level spawn point
	startData = gotoNextChar(startData, end, ',');
	startData++;
	LevelID levelID = static_cast<LevelID>(atoi(startData));
	if (levelID <= LevelID::VOID || levelID >= LevelID::MAX)
	{
		g_logger->logError("MapReader", "Could not read level entry : Level ID not recognized.");
		return false;
	}

	startData = gotoNextChar(startData, end, ',');
	startData++;
	float x = static_cast<float>(atof(startData));
	startData = gotoNextChar(startData, end, ',');
	startData++;
	float y = static_cast<float>(atof(startData));
	sf::Vector2f spawnPoint(x, y);
	
	MapExitBean levelEntry;
	levelEntry.mapExitRect = entry;
	levelEntry.level = levelID;
	levelEntry.levelSpawnPoint = spawnPoint;
	data.levelEntries.push_back(levelEntry);
	return true;
}

bool MapReader::readMapSize(char* start, char* end, MapData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	int width = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	int height = atoi(startData);
	sf::Vector2i size(width, height);
	data.mapSize = size;
	return true;
}
bool MapReader::readTileSize(char* start, char* end, MapData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	int width = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	int height = atoi(startData);
	sf::Vector2i size(width, height);
	data.tileSize = size;
	return true;
}

bool MapReader::readLayerNPC(char* start, char* end, MapData& data) const
{
	char* startData;
	char* endData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	endData = gotoNextChar(startData, end, '"');

	int firstgid = atoi(startData);
	startData = gotoNextChar(start, end, ',');
	startData++;

	NPCID npc;
	while (startData != NULL)
	{
		int id = atoi(startData);
		id = (id == 0) ? id : id - firstgid + 1;
		if (m_npcMap.find(id) == m_npcMap.end())
		{
			g_logger->logError("MapReader", "Npc ID not recognized: " + std::to_string(id));
			return false;
		}
		npc = m_npcMap.at(id);
		data.npcs.push_back(npc);
		startData = gotoNextChar(startData, endData, ',');
		if (startData != NULL)
		{
			startData++;
		}
	}
	return true;
}

bool MapReader::readBackgroundLayerTiles(char* start, char* end, MapData& data) const
{
	// add a new background layer into data
	vector<int> layer;

	char* startData;
	char* endData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	endData = gotoNextChar(startData, end, '"');


	while (startData != NULL)
	{
		layer.push_back(atoi(startData));
		startData = gotoNextChar(startData, endData, ',');
		if (startData != NULL)
		{
			startData++;
		}
	}

	data.backgroundLayers.push_back(layer);
	return true;
}

bool MapReader::readForegroundLayerTiles(char* start, char* end, MapData& data) const
{
	// add a new foreground layer into data
	vector<int> layer;

	char* startData;
	char* endData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	endData = gotoNextChar(startData, end, '"');


	while (startData != NULL)
	{
		layer.push_back(atoi(startData));
		startData = gotoNextChar(startData, endData, ',');
		if (startData != NULL)
		{
			startData++;
		}
	}

	data.foregroundLayers.push_back(layer);
	return true;
}

bool MapReader::readLayerCollidable(char* start, char* end, MapData& data) const
{
	data.collidableTiles.clear();

	char* startData;
	char* endData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	endData = gotoNextChar(startData, end, '"');


	while (startData != NULL)
	{
		data.collidableTiles.push_back(0 != atoi(startData));
		startData = gotoNextChar(startData, endData, ',');
		if (startData != NULL)
		{
			startData++;
		}
	}

	return true;
}

bool MapReader::readMap(char* fileName, MapData& data)
{
	FILE* mapFile;
	mapFile = fopen(fileName, "r");

	if (mapFile == NULL)
	{
		g_logger->logError("MapReader", "Error at opening file " + std::string(fileName));
		return false;
	}

	// obtain file size
	long fileSize;
	long returnedFileSize;

	fseek(mapFile, 0, SEEK_END);
	fileSize = ftell(mapFile);
	rewind(mapFile);

	// huge buffer array
	char* charBuffer = new char[fileSize];

	// fill buffer array
	returnedFileSize = static_cast<long>(fread(charBuffer, sizeof(char), fileSize, mapFile));

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

		else if (strncmp(pos, MAP_NAME, strlen(MAP_NAME)) == 0) {
			g_logger->log(LogLevel::Verbose, "MapReader", "found tag " + std::string(MAP_NAME));
			noError = readMapName(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, MAP_SIZE, strlen(MAP_SIZE)) == 0) {
			g_logger->log(LogLevel::Verbose, "MapReader", "found tag " + std::string(MAP_SIZE));
			noError = readMapSize(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, MAP_TILESIZE, strlen(MAP_TILESIZE)) == 0) {
			g_logger->log(LogLevel::Verbose, "MapReader", "found tag " + std::string(MAP_TILESIZE));
			noError = readTileSize(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, TILESET_PATH, strlen(TILESET_PATH)) == 0) {
			g_logger->log(LogLevel::Verbose, "MapReader", "found tag " + std::string(TILESET_PATH));
			noError = readTilesetPath(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LEVEL_ENTRY, strlen(LEVEL_ENTRY)) == 0) {
			g_logger->log(LogLevel::Verbose, "MapReader", "found tag " + std::string(LEVEL_ENTRY));
			noError = readLevelEntry(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LAYER_COLLIDABLE, strlen(LAYER_COLLIDABLE)) == 0) {
			g_logger->log(LogLevel::Verbose, "MapReader", "found tag " + std::string(LAYER_COLLIDABLE));;
			noError = readLayerCollidable(pos, end, data);
			pos = gotoNextChar(pos, end, ';');
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LAYER_TILES_BACKGROUND, strlen(LAYER_TILES_BACKGROUND)) == 0) {
			g_logger->log(LogLevel::Verbose, "MapReader", "found tag " + std::string(LAYER_TILES_BACKGROUND));
			noError = readBackgroundLayerTiles(pos, end, data);
			pos = gotoNextChar(pos, end, ';');
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LAYER_TILES_FOREGROUND, strlen(LAYER_TILES_FOREGROUND)) == 0) {
			g_logger->log(LogLevel::Verbose, "MapReader", "found tag " + std::string(LAYER_TILES_FOREGROUND));
			noError = readForegroundLayerTiles(pos, end, data);
			pos = gotoNextChar(pos, end, ';');
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LAYER_NPC, strlen(LAYER_NPC)) == 0) {
			g_logger->log(LogLevel::Verbose, "MapReader", "found tag " + std::string(LAYER_NPC));
			noError = readLayerNPC(pos, end, data);
			pos = gotoNextChar(pos, end, ';');
			pos = gotoNextChar(pos, end, '\n');
		}
		else {
			g_logger->logError("MapReader", "unknown tag found in file " + std::string(fileName));
			return false;
		}

		if (pos == NULL || !noError)
		{
			// reached end of file or error happened.
			break;
		}

		pos++;
	}

	std::fclose(mapFile);
	delete[] charBuffer;

	// check data
	if (!noError || !checkData(data))
	{
		return false;
	}

	updateData(data);

	return true;
}

void MapReader::updateData(MapData& data) const
{
	int x = 0;
	int y = 0;

	vector<bool> xLine;

	// calculate collidable tiles
	for (std::vector<bool>::iterator it = data.collidableTiles.begin(); it != data.collidableTiles.end(); ++it) {

		xLine.push_back((*it));
		if (x + 1 >= data.mapSize.x)
		{
			x = 0;
			y++;
			data.collidableTileRects.push_back(xLine); // push back creates a copy of that vector.
			xLine.clear();
		}
		else
		{
			x++;
		}
	}

	x = 0;
	y = 0;

	int tileWidth = data.tileSize.x;
	int tileHeight = data.tileSize.y;

	// calculate npcs
	for (std::vector<NPCID>::iterator it = data.npcs.begin(); it != data.npcs.end(); ++it)
	{
		NPCID id = (*it);		if (id != NPCID::Void)
		{
			data.npcPositions.push_back(std::pair<NPCID, sf::Vector2f>(id, sf::Vector2f(static_cast<float>(x * tileWidth), static_cast<float>(y * tileHeight))));
		}
		if (x + 1 >= data.mapSize.x)
		{
			x = 0;
			y++;
		}
		else
		{
			x++;
		}
	}

	// update level entries
	vector<MapExitBean> oldExits = data.levelEntries;
	data.levelEntries.clear();
	for (auto it : oldExits)
	{
		MapExitBean newBean;
		newBean.mapExitRect = sf::FloatRect(
			it.mapExitRect.left * data.tileSize.x,
			it.mapExitRect.top * data.tileSize.y,
			it.mapExitRect.width * data.tileSize.x,
			it.mapExitRect.height * data.tileSize.y);
		newBean.level = it.level;
		newBean.levelSpawnPoint = sf::Vector2f(
			it.levelSpawnPoint.x * data.tileSize.x,
			it.levelSpawnPoint.y * data.tileSize.y);
		data.levelEntries.push_back(newBean);
	}

	// calculate map rect
	data.mapRect.left = 0;
	data.mapRect.top = 0;
	data.mapRect.height = static_cast<float>(data.tileSize.y * data.mapSize.y);
	data.mapRect.width = static_cast<float>(data.tileSize.x * data.mapSize.x);
}