#include "LevelReader.h"

using namespace std;

//constructor
LevelReader::LevelReader()
{
}

//destructor
LevelReader::~LevelReader()
{
}

char* LevelReader::gotoNextChar(char* buffer, char* end, char goal)
{
	while (buffer < end && *buffer != goal)
	{
		buffer++;
	}

	// goal not found
	if (buffer >= end)
	{
		return NULL;
	}

	return buffer;
}

int LevelReader::countToNextChar(char* buffer, char* end, char goal)
{
	int count = 0;
	while (buffer < end && *buffer != goal)
	{
		buffer++;
		count++;
	}

	// goal not found
	if (buffer >= end)
	{
		return -1;
	}

	return count;
}

bool LevelReader::checkData(LevelData& data)
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
	if (data.startPos.x < 0 || data.startPos.x > data.mapSize.x || data.startPos.y < 0 || data.startPos.y > data.mapSize.y)
	{
		g_logger->logError("LevelReader", "Error in level data : invalid start position, must be in range of map");
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
	if (data.layers.empty())
	{
		g_logger->logError("LevelReader", "Error in level data : no layers set");
		return false;
	}
	for (int i = 0; i < data.layers.size(); i++)
	{
		if (data.layers[i].empty())
		{
			g_logger->logError("LevelReader", "Error in level data : layer " + i + std::string(" empty"));
			return false;
		}
		if (data.layers[i].size() != data.mapSize.x * data.mapSize.y)
		{
			g_logger->logError("LevelReader", "Error in level data : layer " + i + std::string(" has not correct size (map size)"));
			return false;
		}
	}
	for (int i = 0; i < data.dynamicTiles.size(); i++)
	{
		if (data.dynamicTiles[i].first == DynamicTileID::Void)
		{
			g_logger->logError("LevelReader", "Error in level data : dynamic tile ID not recognized");
			return false;
		}
		if (data.dynamicTiles[i].second.empty() || data.dynamicTiles[i].second.size() != data.mapSize.x * data.mapSize.y)
		{
			g_logger->logError("LevelReader", "Error in level data : dynamic tile layer dynamic tile layer has not correct size (map size)");
			return false;
		}
	}
	if (data.collidableTiles.empty())
	{
		g_logger->logError("LevelReader", "Error in level data : collidable layer is empty");
		return false;
	}
	if (data.collidableTiles.size() != data.mapSize.x * data.mapSize.y)
	{
		g_logger->logError("LevelReader", "Error in level data : collidable layer has not correct size (map size)");
		return false;
	}
	
	return true;
}

bool LevelReader::readLevelName(char* start, char* end, LevelData& data)
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

bool LevelReader::readTilesetPath(char* start, char* end, LevelData& data)
{
	char* startData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	string path(startData);
	int count = countToNextChar(startData, end, '"');
	if (count == -1) {
		return false;
	}
	path = path.substr(0, count);
	data.tileSetPath = path;
	return true;
}

bool LevelReader::readLayerBackground(char* start, char* end, LevelData& data)
{
	char* startData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	float distance = static_cast<float>(atof(startData));
	startData = gotoNextChar(start, end, ',');
	startData++;
	string path(startData);
	int count = countToNextChar(startData, end, '"');
	if (count == -1) {
		return false;
	}
	path = path.substr(0, count);

	BackgroundLayer layer;
	layer.load(path, distance);
	data.backgroundLayers.push_back(layer);
	return true;
}

bool LevelReader::readMapSize(char* start, char* end, LevelData& data)
{
	char* startData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	int width = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	int height = atoi(startData);
	sf::Vector2i size(width, height);
	data.mapSize = size;
	return true;
}
bool LevelReader::readTileSize(char* start, char* end, LevelData& data)
{
	char* startData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	int width = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	int height = atoi(startData);
	sf::Vector2i size(width, height);
	data.tileSize = size;
	return true;
}

bool LevelReader::readLayerTiles(char* start, char* end, LevelData& data)
{
	// add a new layer into data
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
	
	data.layers.push_back(layer);
	return true;
}

bool LevelReader::readLayerCollidable(char* start, char* end, LevelData& data)
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

bool LevelReader::readLayerDynamicTiles(char* start, char* end, LevelData& data)
{
	char* startData;
	char* endData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	endData = gotoNextChar(startData, end, '"');

	// read dynamic tile id
	DynamicTileID id = resolveDynamicTile(atoi(startData));
	vector<bool> dynamicTiles;
	startData = gotoNextChar(startData, endData, ',');
	startData++;

	while (startData != NULL)
	{
		dynamicTiles.push_back(0 != atoi(startData));
		startData = gotoNextChar(startData, endData, ',');
		if (startData != NULL)
		{
			startData++;
		}
	}

	data.dynamicTiles.push_back(std::pair<DynamicTileID, vector<bool>>(id, dynamicTiles));

	return true;
}

bool LevelReader::readStartPos(char* start, char* end, LevelData& data)
{
	char* startData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	int x = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	int y = atoi(startData);
	sf::Vector2f pos(static_cast<float>(x), static_cast<float>(y));
	data.startPos = pos;
	return true;
}

bool LevelReader::readLevel(char* fileName, LevelData& data)
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

	// read defined tags
	while (pos < end)
	{
		if (*pos == COMMENT_MARKER)
		{
			pos = gotoNextChar(pos, end, '\n');
		}

		else if (strncmp(pos, LEVEL_NAME, strlen(LEVEL_NAME)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + LEVEL_NAME);
			readLevelName(pos, end, data);			
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, MAP_SIZE, strlen(MAP_SIZE)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + MAP_SIZE);
			readMapSize(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, MAP_TILESIZE, strlen(MAP_TILESIZE)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + MAP_TILESIZE);
			readTileSize(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, TILESET_PATH, strlen(TILESET_PATH)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + TILESET_PATH);
			readTilesetPath(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LAYER_COLLIDABLE, strlen(LAYER_COLLIDABLE)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + LAYER_COLLIDABLE);
			readLayerCollidable(pos, end, data);
			pos = gotoNextChar(pos, end, ';');
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LAYER_TILES, strlen(LAYER_TILES)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + LAYER_TILES);
			readLayerTiles(pos, end, data);
			pos = gotoNextChar(pos, end, ';');
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LAYER_DYNAMIC_TILES, strlen(LAYER_DYNAMIC_TILES)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + LAYER_DYNAMIC_TILES);
			readLayerDynamicTiles(pos, end, data);
			pos = gotoNextChar(pos, end, ';');
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, CENDRIC_STARTPOS, strlen(CENDRIC_STARTPOS)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + CENDRIC_STARTPOS);
			readStartPos(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LAYER_BACKGROUND, strlen(LAYER_BACKGROUND)) == 0) {
			g_logger->log(LogLevel::Verbose, "LevelReader", std::string("Found tag ") + LAYER_BACKGROUND);
			readLayerBackground(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else {
			g_logger->logError("LevelReader", std::string("Uknown tag found in file ") + fileName);
			return false;
		}

		if (pos == NULL) 
		{
			// reached end of file
			break;
		}

		pos++;
	}
	
	std::fclose(levelFile);
	delete[] charBuffer;

	// check data
	if (!checkData(data))
	{
		return false;
	}

	updateData(data);

	return true;
}

void LevelReader::updateData(LevelData& data) 
{
	// update start pos
	data.startPos.x = data.startPos.x * data.tileSize.x;
	data.startPos.y = data.startPos.y * data.tileSize.y;

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
			data.collidableTileRects.push_back(xLine); // push back creates a copy of that vector.
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

	for (std::vector<std::pair<DynamicTileID, std::vector<bool>>>::iterator it = data.dynamicTiles.begin(); it != data.dynamicTiles.end(); ++it) 
	{
		int x = 0;
		int y = 0;
		DynamicTileID id = it->first;
		for (std::vector<bool>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			if ((*it2))
			{
				data.dynamicTileRects.push_back(std::pair<DynamicTileID, sf::Vector2f>(id, sf::Vector2f(static_cast<float>(x * tileWidth), static_cast<float>(y * tileHeight))));
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

	// calculate level rect
	data.levelRect.left = 0;
	data.levelRect.top = 0;
	data.levelRect.height = static_cast<float>(data.tileSize.y * data.mapSize.y);
	data.levelRect.width = static_cast<float>(data.tileSize.x * data.mapSize.x);
}

DynamicTileID LevelReader::resolveDynamicTile(int tileID)
{
	switch (tileID)
	{
	case 1:
		return DynamicTileID::Water;
	case 2:
		return DynamicTileID::Ice;
	case 3:
		return DynamicTileID::Crumbly_block;
	default:
		return DynamicTileID::Void;
	}
}