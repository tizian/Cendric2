#include "ObjectFactory.h"

#include "Level/Enemy.h"
#include "Spells/Spell.h"
#include "Level/LevelDynamicTile.h"
#include "Map/MapDynamicTile.h"

ObjectFactory* ObjectFactory::Instance() {
	static ObjectFactory factory;
	return &factory;
}

void ObjectFactory::registerEnemy(EnemyID id, EnemyConstructor constructor) {
	enemyRegistry.insert({ id, constructor });
}

Enemy* ObjectFactory::createEnemy(EnemyID id, const Level* level, Screen* screen) {
	Enemy* instance = nullptr;
	const auto& it = enemyRegistry.find(id);
	if (it != enemyRegistry.end()) {
		instance = it->second(level, screen);
	}
	return instance;
}

void ObjectFactory::registerLevelDynamicTile(LevelDynamicTileID id, LevelDynamicTileConstructor constructor) {
	levelDynamicTileRegistry.insert({ id, constructor });
}

LevelDynamicTile* ObjectFactory::createLevelDynamicTile(LevelDynamicTileID id, LevelScreen* levelScreen) {
	LevelDynamicTile* instance = nullptr;

	const auto& it = levelDynamicTileRegistry.find(id);
	if (it != levelDynamicTileRegistry.end()) {
		instance = it->second(levelScreen);
	}
	return instance;
}

void ObjectFactory::registerMapDynamicTile(MapDynamicTileID id, MapDynamicTileConstructor constructor) {
	mapDynamicTileRegistry.insert({ id, constructor });
}

MapDynamicTile* ObjectFactory::createMapDynamicTile(MapDynamicTileID id, MapScreen* mapScreen) {
	MapDynamicTile* instance = nullptr;
	const auto& it = mapDynamicTileRegistry.find(id);
	if (it != mapDynamicTileRegistry.end()) {
		instance = it->second(mapScreen);
	}
	return instance;
}