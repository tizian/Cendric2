#include <stdafx.h>

#define __COMMENT_MARKER '#'
#define __LEVEL_NAME "level.name"
#define __MAP_SIZE "map.size"
#define __MAP_TILESIZE "map.tilesize"
#define __TILESET_PATH "tileset.path"
#define __LAYER_COLLIDABLE "layer.collidable"
#define __LAYER_TILES "layer.tiles"
#define __LAYER_BACKGROUND "layer.background"
#define __CENDRIC_STARTPOS "cendric.startpos"

struct LevelData
{
	string name;
	Vector2i mapSize;
	Vector2i tileSize;
	Vector2f startPos;
	string tileSetPath;
	vector<BackgroundLayer> backgroundLayers;
	vector<vector<int> > layers;
	vector<bool> collidableTiles;
	vector<sf::FloatRect> collidableTileRects;
	FloatRect levelRect;
};

class LevelReader
{
public:
	LevelReader();
	~LevelReader();

	bool readLevel(char* fileName, LevelData& data);

private:
	Level* m_pLevel;

	char* gotoNextChar(char* buffer, char* end, char goal);
	int countToNextChar(char* buffer, char* end, char goal);

	bool readLevelName(char* start, char* end, LevelData& data);
	bool readTilesetPath(char* start, char* end, LevelData& data);
	bool readMapSize(char* start, char* end, LevelData& data);
	bool readTileSize(char* start, char* end, LevelData& data);
	bool readLayerBackground(char* start, char* end, LevelData& data);
	bool readLayerCollidable(char* start, char* end, LevelData& data);
	bool readStartPos(char* start, char* end, LevelData& data);
	bool readLayerTiles(char* start, char* end, LevelData& data);

	// \brief check level bean for validity before loading the level
	bool checkData(LevelData& data);

	// \brief update data to prepare it for the level
	void updateData(LevelData& data);
};