#include "Registrar.h"

Registrar::Registrar(EnemyID id, EnemyConstructor constructor) {
	ObjectFactory::registerEnemy(id, constructor);
}

Registrar::Registrar(LevelDynamicTileID id, LevelDynamicTileConstructor constructor) {
	ObjectFactory::registerLevelDynamicTile(id, constructor);
}

Registrar::Registrar(MapDynamicTileID id, MapDynamicTileConstructor constructor) {
	ObjectFactory::registerMapDynamicTile(id, constructor);
}