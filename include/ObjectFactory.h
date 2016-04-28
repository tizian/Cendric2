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
class LevelScreen;
class MapScreen;
class Level;
class Screen;

typedef std::function<Enemy*(const Level*, Screen*)> EnemyConstructor;
typedef std::function<LevelDynamicTile*(LevelScreen*)> LevelDynamicTileConstructor;
typedef std::function<MapDynamicTile*(MapScreen*)> MapDynamicTileConstructor;

class ObjectFactory {
public:
	static ObjectFactory* Instance();

	Enemy* createEnemy(EnemyID id, const Level* level, Screen* screen);
	void registerEnemy(EnemyID id, EnemyConstructor constructor);

	LevelDynamicTile* createLevelDynamicTile(LevelDynamicTileID id, LevelScreen* level);
	void registerLevelDynamicTile(LevelDynamicTileID id, LevelDynamicTileConstructor constructor);

	MapDynamicTile* createMapDynamicTile(MapDynamicTileID id, MapScreen* map);
	void registerMapDynamicTile(MapDynamicTileID id, MapDynamicTileConstructor constructor);

private:
	std::map<EnemyID, EnemyConstructor> enemyRegistry;
	std::map<LevelDynamicTileID, LevelDynamicTileConstructor> levelDynamicTileRegistry;
	std::map<MapDynamicTileID, MapDynamicTileConstructor> mapDynamicTileRegistry;
};