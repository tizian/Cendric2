#pragma once

#include "global.h"
#include "Enums/EnemyID.h"
#include "Enums/SpellID.h"
#include "Enums/MapDynamicTileID.h"
#include "Enums/LevelDynamicTileID.h"
#include "Steam/CendricAchievements.h"

class Enemy;
class Spell;
class LevelDynamicTile;
class MapDynamicTile;
class LevelScreen;
class MapScreen;
class Level;
class Screen;
class Achievement;

typedef std::function<Enemy*(const Level*, Screen*)> EnemyConstructor;
typedef std::function<LevelDynamicTile*(LevelScreen*)> LevelDynamicTileConstructor;
typedef std::function<MapDynamicTile*(MapScreen*)> MapDynamicTileConstructor;
typedef std::function<Achievement*()> AchievementConstructor;

class ObjectFactory final {
private:
	ObjectFactory() {}
public:
	static ObjectFactory* Instance();

	Enemy* createEnemy(EnemyID id, const Level* level, Screen* screen);
	void registerEnemy(EnemyID id, EnemyConstructor constructor);

	LevelDynamicTile* createLevelDynamicTile(LevelDynamicTileID id, LevelScreen* level);
	void registerLevelDynamicTile(LevelDynamicTileID id, LevelDynamicTileConstructor constructor);

	MapDynamicTile* createMapDynamicTile(MapDynamicTileID id, MapScreen* map);
	void registerMapDynamicTile(MapDynamicTileID id, MapDynamicTileConstructor constructor);

	Achievement* createAchievement(AchievementID id);
	void registerAchievement(AchievementID id, AchievementConstructor constructor);

private:
	std::map<EnemyID, EnemyConstructor> enemyRegistry;
	std::map<LevelDynamicTileID, LevelDynamicTileConstructor> levelDynamicTileRegistry;
	std::map<MapDynamicTileID, MapDynamicTileConstructor> mapDynamicTileRegistry;
	std::map<AchievementID, AchievementConstructor> achievementRegistry;
};