#include "Registrar.h"

Registrar::Registrar(EnemyID id, EnemyConstructor constructor) {
	ObjectFactory::Instance()->registerEnemy(id, constructor);
}

Registrar::Registrar(LevelDynamicTileID id, LevelDynamicTileConstructor constructor) {
	ObjectFactory::Instance()->registerLevelDynamicTile(id, constructor);
}

Registrar::Registrar(MapDynamicTileID id, MapDynamicTileConstructor constructor) {
	ObjectFactory::Instance()->registerMapDynamicTile(id, constructor);
}

Registrar::Registrar(AchievementID id, AchievementConstructor constructor) {
	ObjectFactory::Instance()->registerAchievement(id, constructor);
}