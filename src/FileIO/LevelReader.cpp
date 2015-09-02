#include "FileIO/LevelReader.h"

#include <cstring>

using namespace std;

LevelReader::LevelReader()
{
	initMaps();
}

LevelReader::~LevelReader()
{
	m_levelItemMap.clear();
	m_enemyMap.clear();
}

void LevelReader::initMaps()
{
	m_levelItemMap.insert({
		{ 0, "" },
		{ 1, "it_fo_caveberry" },
		{ 2, "it_mi_goldengoblet" },
		{ 3, "it_go_onecoin" },
		{ 4, "it_qe_letter" },
		{ 5, "it_eq_ringoflesserhealth" },
		{ 26, "it_fo_healingherb" },
		{ 27, "it_fo_glowingshroom" }, 
		{ 28, "it_go_threecoins" },
		{ 29, "it_fo_water" },
		{ 49, "it_fo_cheese" },
		{ 50, "it_fo_bread" },
	});
	m_enemyMap.insert({
		{ 0, EnemyID::Void },
		{ 1, EnemyID::Rat },
		{ 2, EnemyID::FireRat },
	});
}

bool LevelReader::checkData(LevelData& data) const
{
	if (data.mapSize.x == 0 || data.mapSize.y == 0)
	{
		g_logger->logError("LevelReader", "Error in level data : map size not set / invalid");
		return false;
	}
	if (data.tileSize.x == 0 || data.tileSize.y == 0)
	{
		g_logger->logError("LevelReader", "Error in level data: tile size not set / invalid");
		return false;
	}
	if (data.name.empty())
	{
		g_logger->logError("LevelReader", "Error in level data : level name not set / empty");
		return false;
	}
	if (data.tileSetPath.empty())
	{
		g_logger->logError("LevelReader", "Error in level data : tileset-path not set / empty");
		return false;
	}
	if (data.backgroundTileLayers.empty())
	{
		g_logger->logInfo("LevelReader", "No background tile layers set");
	}
	for (int i = 0; i < data.backgroundTileLayers.size(); i++)
	{
		if (data.backgroundTileLayers[i].empty())
		{
			g_logger->logError("LevelReader", "Error in level data : background tile layer " + std::to_string(i) + std::string(" empty"));
			return false;
		}
		if (data.backgroundTileLayers[i].size() != data.mapSize.x * data.mapSize.y)
		{
			g_logger->logError("LevelReader", "Error in level data : background tile layer " + std::to_string(i) + std::string(" has not correct size (map size)"));
			return false;
		}
	}
	if (data.foregroundTileLayers.empty())
	{
		g_logger->logInfo("LevelReader", "No foreground tile layers set");
	}
	for (int i = 0; i < data.foregroundTileLayers.size(); i++)
	{
		if (data.foregroundTileLayers[i].empty())
		{
			g_logger->logError("LevelReader", "Error in level data : foreground tile layer " + std::to_string(i) + std::string(" empty"));
			return false;
		}
		if (data.foregroundTileLayers[i].size() != data.mapSize.x * data.mapSize.y)
		{
			g_logger->logError("LevelReader", "Error in level data : foreground tile layer " + std::to_string(i) + std::string(" has not correct size (map size)"));
			return false;
		}
	}
	for (int i = 0; i < data.dynamicTileLayers.size(); i++)
	{
		if (data.dynamicTileLayers[i].first == DynamicTileID::VOID)
		{
			g_logger->logError("LevelReader", "Error in level data : dynamic tile ID not recognized");
			return false;
		}
		if (data.dynamicTileLayers[i].second.empty() || data.dynamicTileLayers[i].second.size() != data.mapSize.x * data.mapSize.y)
		{
			g_logger->logError("LevelReader", "Error in level data : dynamic tile layer has not correct size (map size)");
			return false;
		}
	}
	for (auto it : data.levelExits)
	{
		if (it.levelExitRect.height <= 0.0 || it.levelExitRect.width <= 0.0)
		{
			g_logger->logError("LevelReader", "Error in level data : level exit rectangle has volume negative or null.");
			return false;
		}
	}
	if (data.levelItems.size() != data.mapSize.x * data.mapSize.y)
	{
		g_logger->logError("LevelReader", "Error in level data : level item layer has not correct size (map size)");
		return false;
	}
	if (data.enemies.size() != data.mapSize.x * data.mapSize.y)
	{
		g_logger->logError("LevelReader", "Error in level data : enemy layer has not correct size (map size)");
		return false;
	}
	if (data.collidableTiles.empty())
	{
		g_logger->logError("LevelReader", "Error in level data : collidable layer is empty (can be all zeros but must be set)");
		return false;
	}
	if (data.collidableTiles.size() != data.mapSize.x * data.mapSize.y)
	{
		g_logger->logError("LevelReader", "Error in level data : collidable layer has not correct size (map size)");
		return false;
	}
	
	return true;
}

bool LevelReader::readLevelName(char* start, char* end, LevelData& data) const
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
	data.name = name;
	return true;
}

bool LevelReader::readTilesetPath(char* start, char* end, LevelData& data) const
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

bool LevelReader::readLayerBackground(char* start, char* end, LevelData& data) const
{
	char* startData;
	startData = gotoNextChar(start, end, ':');
	startData++;
	float distance = static_cast<float>(atof(startData));
	startData = gotoNextChar(start, end, ',');
	startData++;
	string path(startData);
	int count = countToNextChar(startData, end, '\n');
	if (count == -1) {
		return false;
	}
	path = path.substr(0, count);

	BackgroundLayer layer;
	layer.load(path, distance);
	data.backgroundLayers.push_back(layer);
	return true;
}

bool LevelReader::readMapSize(char* start, char* end, LevelData& data) const
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

bool LevelReader::readTileSize(char* start, char* end, LevelData& data) const
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

bool LevelReader::readLevelExit(char* start, char* end, LevelData& data) const
{
	char* startData;
	
	// read level exit rect
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
	sf::FloatRect exit(left, top, width, height);

	// read map spawn point
	startData = gotoNextChar(startData, end, ',');
	startData++;
	MapID mapID = static_cast<MapID>(atoi(startData));
	if (mapID <= MapID::VOID || mapID >= MapID::MAX)
	{
		g_logger->logError("LevelReader", "Could not read level exit : Map ID not recognized.");
		return false;
	}
	startData = gotoNextChar(startData, end, ',');
	startData++;
	float x = static_cast<float>(atof(startData));
	startData = gotoNextChar(startData, end, ',');
	startData++;
	float y = static_cast<float>(atof(startData));
	sf::Vector2f spawnPoint(x, y);
	LevelExitBean levelExit;
	levelExit.levelExitRect = exit;
	levelExit.map = mapID;
	levelExit.mapSpawnPoint = spawnPoint;
	data.levelExits.push_back(levelExit);
	return true;
}

bool LevelReader::readLayerTilesBackground(char* start, char* end, LevelData& data) const
{
	// add a new background tile layer into data
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
	
	data.backgroundTileLayers.push_back(layer);
	return true;
}

bool LevelReader::readLayerTilesForeground(char* start, char* end, LevelData& data) const
{
	// add a new foreground tile layer into data
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

	data.foregroundTileLayers.push_back(layer);
	return true;
}

bool LevelReader::readLayerCollidable(char* start, char* end, LevelData& data) const
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

bool LevelReader::readLayerDynamicTiles(char* start, char* end, LevelData& data) const
{
	char* startData;
	char* endData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	endData = gotoNextChar(startData, end, '"');

	// read dynamic tile id
	DynamicTileID id = static_cast<DynamicTileID>(atoi(startData));
	if (id <= DynamicTileID::VOID || id >= DynamicTileID::MAX)
	{
		g_logger->logError("LevelReader", "Dynamic tile ID not recognized: " + std::to_string(static_cast<int>(id)));
		return false;
	}

	// read offset
	startData = gotoNextChar(startData, endData, ',');
	startData++;
	int offset = atoi(startData);

	vector<int> dynamicTiles;
	startData = gotoNextChar(startData, endData, ',');
	startData++;

	while (startData != NULL)
	{
		int skinNr = atoi(startData);
		dynamicTiles.push_back(skinNr == 0 ? 0 : ((skinNr - offset) / DYNAMIC_TILE_COUNT) + 1);
		startData = gotoNextChar(startData, endData, ',');
		if (startData != NULL)
		{
			startData++;
		}
	}

	data.dynamicTileLayers.push_back(std::pair<DynamicTileID, vector<int>>(id, dynamicTiles));

	return true;
}

bool LevelReader::readLayerLevelItems(char* start, char* end, LevelData& data) const
{
	char* startData;
	char* endData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	endData = gotoNextChar(startData, end, '"');

	int firstgid = atoi(startData);
	startData = gotoNextChar(start, end, ',');
	startData++;

	string levelItem;
	while (startData != NULL)
	{
		int id = atoi(startData);
		id = (id == 0) ? id : id - firstgid + 1;
		if (m_levelItemMap.find(id) == m_levelItemMap.end())
		{
			g_logger->logError("LevelReader", "Level item ID not recognized: " + std::to_string(id));
			return false;
		}
		levelItem = m_levelItemMap.at(id);
		data.levelItems.push_back(levelItem);
		startData = gotoNextChar(startData, endData, ',');
		if (startData != NULL)
		{
			startData++;
		}
	}
	return true;
}

bool LevelReader::readLayerEnemies(char* start, char* end, LevelData& data) const
{
	char* startData;
	char* endData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	endData = gotoNextChar(startData, end, '"');

	int firstgid = atoi(startData);
	startData = gotoNextChar(start, end, ',');
	startData++;

	EnemyID enemy;
	while (startData != NULL)
	{
		int id = atoi(startData);
		id = (id == 0) ? id : id - firstgid + 1;
		if (m_enemyMap.find(id) == m_enemyMap.end())
		{
			g_logger->logError("LevelReader", "Enemy ID not recognized: " + std::to_string(id));
			return false;
		}
		enemy = m_enemyMap.at(id);
		data.enemies.push_back(enemy);
		startData = gotoNextChar(startData, endData, ',');
		if (startData != NULL)
		{
			startData++;
		}
	}
	return true;
}

bool LevelReader::readLevel(char* fileName, LevelData& data) const
{
	FILE* levelFile;
	levelFile = fopen(fileName, "r");

	if (levelFile == NULL)
	{
		g_logger->logError("LevelReader", "Error at opening file " + std::string(fileName));
		return false;
	}

	// obtain file size
	long fileSize;
	long returnedFileSize;

	fseek(levelFile, 0, SEEK_END);
	fileSize = ftell(levelFile);
	rewind(levelFile);

	// huge buffer array
	char* charBuffer = new char[fileSize];

	// fill buffer array
	returnedFileSize = static_cast<long>(fread(charBuffer, sizeof(char), fileSize, levelFile));

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

		else if (strncmp(pos, LEVEL_NAME, strlen(LEVEL_NAME)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + LEVEL_NAME);
			noError = readLevelName(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, MAP_SIZE, strlen(MAP_SIZE)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + MAP_SIZE);
			noError = readMapSize(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, MAP_TILESIZE, strlen(MAP_TILESIZE)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + MAP_TILESIZE);
			noError = readTileSize(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LEVEL_EXIT, strlen(LEVEL_EXIT)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + LEVEL_EXIT);
			noError = readLevelExit(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, TILESET_PATH, strlen(TILESET_PATH)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + TILESET_PATH);
			noError = readTilesetPath(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LAYER_COLLIDABLE, strlen(LAYER_COLLIDABLE)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + LAYER_COLLIDABLE);
			noError = readLayerCollidable(pos, end, data);
			pos = gotoNextChar(pos, end, ';');
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LAYER_TILES_BACKGROUND, strlen(LAYER_TILES_BACKGROUND)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + LAYER_TILES_BACKGROUND);
			noError = readLayerTilesBackground(pos, end, data);
			pos = gotoNextChar(pos, end, ';');
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LAYER_TILES_FOREGROUND, strlen(LAYER_TILES_FOREGROUND)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + LAYER_TILES_FOREGROUND);
			noError = readLayerTilesForeground(pos, end, data);
			pos = gotoNextChar(pos, end, ';');
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LAYER_DYNAMIC_TILES, strlen(LAYER_DYNAMIC_TILES)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + LAYER_DYNAMIC_TILES);
			noError = readLayerDynamicTiles(pos, end, data);
			pos = gotoNextChar(pos, end, ';');
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LAYER_LEVEL_ITEMS, strlen(LAYER_LEVEL_ITEMS)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + LAYER_LEVEL_ITEMS);
			readLayerLevelItems(pos, end, data);
			pos = gotoNextChar(pos, end, ';');
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LAYER_ENEMIES, strlen(LAYER_ENEMIES)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + LAYER_ENEMIES);
			noError = readLayerEnemies(pos, end, data);
			pos = gotoNextChar(pos, end, ';');
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LAYER_BACKGROUND, strlen(LAYER_BACKGROUND)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + LAYER_BACKGROUND);
			noError = readLayerBackground(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else {
			g_logger->logError("LevelReader", std::string("Unknown tag found in file ") + fileName);
			return false;
		}

		if (pos == NULL || !noError) 
		{
			// reached end of file or error happened
			break;
		}

		pos++;
	}
	
	std::fclose(levelFile);
	delete[] charBuffer;

	// check data
	if (!noError || !checkData(data))
	{
		return false;
	}

	updateData(data);

	return true;
}

void LevelReader::updateData(LevelData& data)  const
{
	// update level exits
	vector<LevelExitBean> oldExits = data.levelExits;
	data.levelExits.clear();
	for (auto it : oldExits)
	{
		LevelExitBean newBean;
		newBean.levelExitRect = sf::FloatRect(
			it.levelExitRect.left * data.tileSize.x,
			it.levelExitRect.top * data.tileSize.y,
			it.levelExitRect.width * data.tileSize.x,
			it.levelExitRect.height * data.tileSize.y);
		newBean.map = it.map;
		newBean.mapSpawnPoint = sf::Vector2f(
			it.mapSpawnPoint.x * data.tileSize.x,
			it.mapSpawnPoint.y * data.tileSize.y);
		data.levelExits.push_back(newBean);
	}

	// calculate collidable tiles
	int x = 0;
	int y = 0;
	vector<bool> xLine;

	for (std::vector<bool>::iterator it = data.collidableTiles.begin(); it != data.collidableTiles.end(); ++it) 
	{
		xLine.push_back((*it));
		if (x + 1 >= data.mapSize.x) 
		{
			x = 0;
			y++;
			data.collidableTilePositions.push_back(xLine); // push back creates a copy of that vector.
			xLine.clear();
		}
		else
		{
			x++;
		}
	}

	// calculate dynamic tiles
	int tileWidth = data.tileSize.x;
	int tileHeight = data.tileSize.y;

	for (auto& it : data.dynamicTileLayers)
	{
		int x = 0;
		int y = 0;
		DynamicTileID id = it.first;
		
		if (id == DynamicTileID::Water)
		{
			// handle special dynamic tiles
			bool isReadingWaterTiles = false;
			float waterTileWidth = 0.f;
			DynamicTileBean bean;
			bean.id = id;
			for (auto& it2 : it.second)
			{
				if (it2 != 0)
				{
					if (isReadingWaterTiles)
					{
						waterTileWidth += tileWidth;
					}
					else
					{
						bean.position = sf::Vector2f(static_cast<float>(x * tileWidth), static_cast<float>(y * tileHeight));
						bean.skinNr = it2;
						waterTileWidth = static_cast<float>(tileWidth);
						isReadingWaterTiles = true;
					}
				}
				else
				{
					if (isReadingWaterTiles)
					{
						isReadingWaterTiles = false;
						bean.size = sf::Vector2f(waterTileWidth, static_cast<float>(tileWidth));
						data.dynamicTiles.push_back(bean);
					}
				}
				if (x + 1 >= data.mapSize.x)
				{
					x = 0;
					y++;
					if (isReadingWaterTiles)
					{
						isReadingWaterTiles = false;
						bean.size = sf::Vector2f(waterTileWidth, static_cast<float>(tileWidth));
						data.dynamicTiles.push_back(bean);
					}
				}
				else
				{
					x++;
				}
			}
		}
		else
		{
			// normal tiles
			for (auto& it2 : it.second)
			{
				if (it2 != 0)
				{
					DynamicTileBean bean;
					bean.id = id;
					bean.position = sf::Vector2f(static_cast<float>(x * tileWidth), static_cast<float>(y * tileHeight));
					bean.skinNr = it2;
					data.dynamicTiles.push_back(bean);
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
		}
	}
	
	x = 0;
	y = 0;

	// calculate level items
	for (std::vector<string>::iterator it = data.levelItems.begin(); it != data.levelItems.end(); ++it)
	{
		string id = (*it);
		if (!id.empty())
		{
			data.levelItemPositions.push_back(std::pair<string, sf::Vector2f>(id, sf::Vector2f(static_cast<float>(x * tileWidth), static_cast<float>(y * tileHeight))));
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

	x = 0;
	y = 0;

	// calculate enemies
	for (std::vector<EnemyID>::iterator it = data.enemies.begin(); it != data.enemies.end(); ++it)
	{
		EnemyID id = (*it);		if (id != EnemyID::Void)
		{
			data.enemyPositions.push_back(std::pair<EnemyID, sf::Vector2f>(id, sf::Vector2f(static_cast<float>(x * tileWidth), static_cast<float>(y * tileHeight))));
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

	// calculate level rect
	data.levelRect.left = 0;
	data.levelRect.top = 0;
	data.levelRect.height = static_cast<float>(data.tileSize.y * data.mapSize.y);
	data.levelRect.width = static_cast<float>(data.tileSize.x * data.mapSize.x);
}