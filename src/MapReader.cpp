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

char* MapReader::gotoNextChar(char* buffer, char* end, char goal)
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

int MapReader::countToNextChar(char* buffer, char* end, char goal)
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

bool MapReader::checkData(MapData& data)
{
	if (data.mapSize.x == 0 || data.mapSize.y == 0)
	{
		printf("LevelReader: Error in level data: map size not set / invalid \n");
		return false;
	}
	if (data.tileSize.x == 0 || data.tileSize.y == 0)
	{
		printf("LevelReader: Error in level data: tile size not set / invalid \n");
		return false;
	}
	if (data.startPos.x < 0 || data.startPos.x > data.mapSize.x || data.startPos.y < 0 || data.startPos.y > data.mapSize.y)
	{
		printf("LevelReader: Error in level data: invalid start position, must be in range of map \n");
		return false;
	}
	if (data.name.empty())
	{
		printf("LevelReader: Error in level data: level name not set / empty \n");
		return false;
	}
	if (data.tileSetPath.empty())
	{
		printf("LevelReader: Error in level data: tileset-path not set / empty \n");
		return false;
	}
	if (data.layers.empty())
	{
		printf("LevelReader: Error in level data: no layers set \n");
		return false;
	}
	for (int i = 0; i < data.layers.size(); i++)
	{
		if (data.layers[i].empty())
		{
			printf("LevelReader: Error in level data: layer %d empty \n", i);
			return false;
		}
		if (data.layers[i].size() != data.mapSize.x * data.mapSize.y)
		{
			printf("LevelReader: Error in level data: layer %d has not correct size (map size) \n", i);
			return false;
		}
	}
	if (data.collidableTiles.empty())
	{
		printf("LevelReader: Error in level data: collidable layer is empty \n");
		return false;
	}
	if (data.collidableTiles.size() != data.mapSize.x * data.mapSize.y)
	{
		printf("LevelReader: Error in level data: collidable layer has not correct size (map size) \n");
		return false;
	}

	return true;
}

bool MapReader::readMapName(char* start, char* end, MapData& data)
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

bool MapReader::readTilesetPath(char* start, char* end, MapData& data)
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

bool MapReader::readMapSize(char* start, char* end, MapData& data)
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
bool MapReader::readTileSize(char* start, char* end, MapData& data)
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

bool MapReader::readLayerTiles(char* start, char* end, MapData& data)
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

bool MapReader::readLayerCollidable(char* start, char* end, MapData& data)
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

bool MapReader::readStartPos(char* start, char* end, MapData& data)
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
		printf("MapReader: Error at opening file %s \n", fileName);
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
		if (*pos == __COMMENT_MARKER)
		{
			pos = gotoNextChar(pos, end, '\n');
		}

		else if (strncmp(pos, __MAP_NAME, strlen(__MAP_NAME)) == 0) {
			printf("MapReader: Found tag %s \n", __MAP_NAME);
			readMapName(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, __MAP_SIZE, strlen(__MAP_SIZE)) == 0) {
			printf("MapReader: Found tag %s \n", __MAP_SIZE);
			readMapSize(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, __MAP_TILESIZE, strlen(__MAP_TILESIZE)) == 0) {
			printf("MapReader: Found tag %s \n", __MAP_TILESIZE);
			readTileSize(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, __TILESET_PATH, strlen(__TILESET_PATH)) == 0) {
			printf("MapReader: Found tag %s \n", __TILESET_PATH);
			readTilesetPath(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, __LAYER_COLLIDABLE, strlen(__LAYER_COLLIDABLE)) == 0) {
			printf("MapReader: Found tag %s \n", __LAYER_COLLIDABLE);
			readLayerCollidable(pos, end, data);
			pos = gotoNextChar(pos, end, ';');
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, __LAYER_TILES, strlen(__LAYER_TILES)) == 0) {
			printf("MapReader: Found tag %s \n", __LAYER_TILES);
			readLayerTiles(pos, end, data);
			pos = gotoNextChar(pos, end, ';');
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, __CENDRIC_STARTPOS, strlen(__CENDRIC_STARTPOS)) == 0) {
			printf("MapReader: Found tag %s \n", __CENDRIC_STARTPOS);
			readStartPos(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else {
			printf("MapReader: Error, unknown tag found in file %s \n", fileName);
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

void MapReader::updateData(MapData& data)
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