#include "MapReader.h"

using namespace std;

//constructor
MapReader::MapReader()
{
}

//destructor
MapReader::~MapReader()
{
}

char* MapReader::gotoNextChar(char* buffer, char* end, char goal) const
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

int MapReader::countToNextChar(char* buffer, char* end, char goal) const
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
	if (data.startPos.x < 0 || data.startPos.x > data.mapSize.x || data.startPos.y < 0 || data.startPos.y > data.mapSize.y)
	{
		g_logger->logError("MapReader", "Error in map data : invalid start position, must be in range of map");
		return false;
	}
	if (data.name.empty())
	{
		g_logger->logError("MapReader", "Error in map data : map name not set / empty");
		return false;
	}
	if (data.tileSetPath.empty())
	{
		g_logger->logError("MapReader", "Error in map data : tileset-path not set / empty");
		return false;
	}
	if (data.layers.empty())
	{
		g_logger->logError("MapReader", "Error in map data : no layers set");
		return false;
	}
	for (int i = 0; i < data.layers.size(); i++)
	{
		if (data.layers[i].empty())
		{
			g_logger->logError("MapReader", "Error in map data : layer " + i + std::string(" empty"));
			return false;
		}
		if (data.layers[i].size() != data.mapSize.x * data.mapSize.y)
		{
			g_logger->logError("MapReader", "Error in map data : layer " + i + std::string(" has not correct size (map size)"));
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

bool MapReader::readMapSize(char* start, char* end, MapData& data) const
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
bool MapReader::readTileSize(char* start, char* end, MapData& data) const
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

bool MapReader::readLayerTiles(char* start, char* end, MapData& data) const
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

bool MapReader::readStartPos(char* start, char* end, MapData& data) const
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

	// read defined tags
	while (pos < end)
	{
		if (*pos == COMMENT_MARKER)
		{
			pos = gotoNextChar(pos, end, '\n');
		}

		else if (strncmp(pos, MAP_NAME, strlen(MAP_NAME)) == 0) {
			g_logger->log(LogLevel::Verbose, "MapReader", "found tag " + std::string(MAP_NAME));
			readMapName(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, MAP_SIZE, strlen(MAP_SIZE)) == 0) {
			g_logger->log(LogLevel::Verbose, "MapReader", "found tag " + std::string(MAP_SIZE));
			readMapSize(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, MAP_TILESIZE, strlen(MAP_TILESIZE)) == 0) {
			g_logger->log(LogLevel::Verbose, "MapReader", "found tag " + std::string(MAP_TILESIZE));
			readTileSize(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, TILESET_PATH, strlen(TILESET_PATH)) == 0) {
			g_logger->log(LogLevel::Verbose, "MapReader", "found tag " + std::string(TILESET_PATH));
			readTilesetPath(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LAYER_COLLIDABLE, strlen(LAYER_COLLIDABLE)) == 0) {
			g_logger->log(LogLevel::Verbose, "MapReader", "found tag " + std::string(LAYER_COLLIDABLE));;
			readLayerCollidable(pos, end, data);
			pos = gotoNextChar(pos, end, ';');
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, LAYER_TILES, strlen(LAYER_TILES)) == 0) {
			g_logger->log(LogLevel::Verbose, "MapReader", "found tag " + std::string(LAYER_TILES));
			readLayerTiles(pos, end, data);
			pos = gotoNextChar(pos, end, ';');
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, CENDRIC_STARTPOS, strlen(CENDRIC_STARTPOS)) == 0) {
			g_logger->log(LogLevel::Verbose, "MapReader", "found tag " + std::string(CENDRIC_STARTPOS));
			readStartPos(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else {
			g_logger->logError("MapReader", "unknown tag found in file " + std::string(fileName));
			return false;
		}

		if (pos == NULL)
		{
			// reached end of file
			break;
		}

		pos++;
	}

	std::fclose(mapFile);
	delete[] charBuffer;

	// check data
	if (!checkData(data))
	{
		return false;
	}

	updateData(data);

	return true;
}

void MapReader::updateData(MapData& data) const
{
	// update start pos
	data.startPos.x = data.startPos.x * data.tileSize.x;
	data.startPos.y = data.startPos.y * data.tileSize.y;

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

	// calculate map rect
	data.mapRect.left = 0;
	data.mapRect.top = 0;
	data.mapRect.height = static_cast<float>(data.tileSize.y * data.mapSize.y);
	data.mapRect.width = static_cast<float>(data.tileSize.x * data.mapSize.x);
}