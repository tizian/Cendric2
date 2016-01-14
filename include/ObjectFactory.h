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

typedef std::function<Enemy*(Level*, Screen*, bool)> EnemyConstructor;
typedef std::function<LevelDynamicTile*(Level*)> LevelDynamicTileConstructor;
typedef std::function<MapDynamicTile*(Map*)> MapDynamicTileConstructor;

class ObjectFactory {
public:
	static Enemy* createEnemy(EnemyID id, Level* level, Screen* screen, bool controlled);
	static void registerEnemy(EnemyID id, EnemyConstructor constructor);

	static LevelDynamicTile* createLevelDynamicTile(LevelDynamicTileID id, Level* level);
	static void registerLevelDynamicTile(LevelDynamicTileID id, LevelDynamicTileConstructor constructor);

	static MapDynamicTile* createMapDynamicTile(MapDynamicTileID id, Map* map);
	static void registerMapDynamicTile(MapDynamicTileID id, MapDynamicTileConstructor constructor);

private:
	static std::map<EnemyID, EnemyConstructor> enemyRegistry;
	static std::map<LevelDynamicTileID, LevelDynamicTileConstructor> levelDynamicTileRegistry;
	static std::map<MapDynamicTileID, MapDynamicTileConstructor> mapDynamicTileRegistry;
};