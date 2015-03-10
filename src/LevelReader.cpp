#include "stdafx.h"

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
		printf("LevelReader: Error in level data: map size not set / invalid \n");
		return false;
	}
	if (data.tileSize.x == 0 || data.tileSize.y == 0)
	{
		printf("LevelReader: Error in level data: tile size not set / invalid \n");
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

bool LevelReader::readMapSize(char* start, char* end, LevelData& data)
{
	char* startData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	int width = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	int height = atoi(startData);
	Vector2i size(width, height);
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
	Vector2i size(width, height);
	data.tileSize = size;
	return true;
}

bool LevelReader::readLayer(char* start, char* end, LevelData& data)
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

bool LevelReader::readStartPos(char* start, char* end, LevelData& data)
{
	char* startData;
	startData = gotoNextChar(start, end, '"');
	startData++;
	int x = atoi(startData);
	startData = gotoNextChar(startData, end, ',');
	startData++;
	int y = atoi(startData);
	Vector2i pos(x, y);
	data.startPos = pos;
	return true;
}

bool LevelReader::readLevel(char* fileName, LevelData& data)
{
	FILE* pLevelFile;
	pLevelFile = fopen(fileName, "r");

	if (pLevelFile == NULL)
	{
		printf("Level: Error at opening file %s \n", fileName);
		system("PAUSE");
		return false;
	}

	// obtain file size
	long fileSize;
	long returnedFileSize;

	fseek(pLevelFile, 0, SEEK_END);
	fileSize = ftell(pLevelFile);
	rewind(pLevelFile);

	// huge buffer array
	char* charBuffer = new char[fileSize];

	// fill buffer array
	returnedFileSize = fread(charBuffer, sizeof(char), fileSize, pLevelFile);
//	if (!feof(pLevelFile))
//	{
//		printf("LevelReader: Error at reading file %s, eof not reached \n", fileName);
//		system("PAUSE");
//		return false;
//	}

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

		else if (strncmp(pos, __LEVEL_NAME, strlen(__LEVEL_NAME)) == 0) {
			printf("LevelReader: Found tag %s \n", __LEVEL_NAME);
			readLevelName(pos, end, data);			
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, __MAP_SIZE, strlen(__MAP_SIZE)) == 0) {
			printf("LevelReader: Found tag %s \n", __MAP_SIZE);
			readMapSize(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, __MAP_TILESIZE, strlen(__MAP_TILESIZE)) == 0) {
			printf("LevelReader: Found tag %s \n", __MAP_TILESIZE);
			readTileSize(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, __TILESET_PATH, strlen(__TILESET_PATH)) == 0) {
			printf("LevelReader: Found tag %s \n", __TILESET_PATH);
			readTilesetPath(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, __LAYER_COLLIDABLE, strlen(__LAYER_COLLIDABLE)) == 0) {
			printf("LevelReader: Found tag %s \n", __LAYER_COLLIDABLE);
			// TODO
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, __LAYER_BACKGROUND, strlen(__LAYER_BACKGROUND)) == 0) {
			printf("LevelReader: Found tag %s \n", __LAYER_BACKGROUND);
			readLayer(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else if (strncmp(pos, __CENDRIC_STARTPOS, strlen(__CENDRIC_STARTPOS)) == 0) {
			printf("LevelReader: Found tag %s \n", __CENDRIC_STARTPOS);
			readStartPos(pos, end, data);
			pos = gotoNextChar(pos, end, '\n');
		}
		else {
			printf("LevelReader: Error, unknown tag found in file %s \n", fileName);
			system("PAUSE");
			return false;
		}

		if (pos == NULL) 
		{
			// reached end of file
			break;
		}

		pos++;
	}
	
	std::fclose(pLevelFile);
	delete[] charBuffer;

	// check data
	if (!checkData(data))
	{
		return false;
	}

	return true;
}