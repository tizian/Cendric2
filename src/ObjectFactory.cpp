#include "ObjectFactory.h"

#include "Level/Enemy.h"
#include "Spell.h"
#include "Level/LevelDynamicTile.h"
#include "Map/MapDynamicTile.h"

std::map<EnemyID, EnemyConstructor> ObjectFactory::enemyRegistry;
std::map<LevelDynamicTileID, LevelDynamicTileConstructor> ObjectFactory::levelDynamicTileRegistry;
std::map<MapDynamicTileID, MapDynamicTileConstructor> ObjectFactory::mapDynamicTileRegistry;

ObjectFactory* ObjectFactory::Instance() {
	static ObjectFactory factory;
	return &factory;
}

void ObjectFactory::registerEnemy(EnemyID id, EnemyConstructor constructor) {
	enemyRegistry.insert({ id, constructor });
}

Enemy* ObjectFactory::createEnemy(EnemyID id, Level* level, Screen* screen, bool controlled) {
	Enemy* instance = nullptr;
	auto& it = enemyRegistry.find(id);
	if (it != enemyRegistry.end()) {
		instance = it->second(level, screen, controlled);
	}
	return instance;
}

void ObjectFactory::registerLevelDynamicTile(LevelDynamicTileID id, LevelDynamicTileConstructor constructor) {
	levelDynamicTileRegistry.insert({ id, constructor });
}

LevelDynamicTile* ObjectFactory::createLevelDynamicTile(LevelDynamicTileID id, Level* level) {
	LevelDynamicTile* instance = nullptr;
	auto& it = levelDynamicTileRegistry.find(id);
	if (it != levelDynamicTileRegistry.end()) {
		instance = it->second(level);
	}
	return instance;
}

void ObjectFactory::registerMapDynamicTile(MapDynamicTileID id, MapDynamicTileConstructor constructor) {
	mapDynamicTileRegistry.insert({ id, constructor });
}

MapDynamicTile* ObjectFactory::createMapDynamicTile(MapDynamicTileID id, Map* map) {
	MapDynamicTile* instance = nullptr;
	auto& it = mapDynamicTileRegistry.find(id);
	if (it != mapDynamicTileRegistry.end()) {
		instance = it->second(map);
	}
	return instance;
}