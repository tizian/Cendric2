#pragma once

#include "global.h"
#include "Enums/EnemyID.h"
#include "Enums/SpellID.h"
#include "Enums/MapDynamicTileID.h"
#include "Enums/LevelDynamicTileID.h"

class Enemy;
class Spell;
class LevelDynamicTile;
class MapDynamicTile;
class Level;
class Map;
class Screen;

typedef std::function<Enemy*(Level*, Screen*)> EnemyConstructor;
typedef std::function<LevelDynamicTile*(Level*)> LevelDynamicTileConstructor;
typedef std::function<MapDynamicTile*(Map*)> MapDynamicTileConstructor;

class ObjectFactory {
public:
	static ObjectFactory* ObjectFactory::Instance();

	Enemy* createEnemy(EnemyID id, Level* level, Screen* screen);
	void registerEnemy(EnemyID id, EnemyConstructor constructor);

	LevelDynamicTile* createLevelDynamicTile(LevelDynamicTileID id, Level* level);
	void registerLevelDynamicTile(LevelDynamicTileID id, LevelDynamicTileConstructor constructor);

	MapDynamicTile* createMapDynamicTile(MapDynamicTileID id, Map* map);
	void registerMapDynamicTile(MapDynamicTileID id, MapDynamicTileConstructor constructor);

private:
	std::map<EnemyID, EnemyConstructor> enemyRegistry;
	std::map<LevelDynamicTileID, LevelDynamicTileConstructor> levelDynamicTileRegistry;
	std::map<MapDynamicTileID, MapDynamicTileConstructor> mapDynamicTileRegistry;
};