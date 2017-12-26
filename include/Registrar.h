#pragma once

#include "global.h"
#include "ObjectFactory.h"

#define REGISTER_ENEMY(ID, TYPE) \
    static Registrar registrar(ID, \
        [](const Level* l, Screen* s) -> Enemy* { return new TYPE(l, s);});

#define REGISTER_LEVEL_DYNAMIC_TILE(ID, TYPE) \
    static Registrar registrar(ID, \
        [](LevelScreen* l) -> LevelDynamicTile* { return new TYPE(l);});

#define REGISTER_MAP_DYNAMIC_TILE(ID, TYPE) \
    static Registrar registrar(ID, \
        [](MapScreen* m) -> MapDynamicTile* { return new TYPE(m);});

#define REGISTER_ACHIEVEMENT(ID, TYPE) \
    static Registrar registrar(ID, \
        []() -> Achievement* { return new TYPE();});

class Registrar final {
public:
	Registrar(EnemyID id, EnemyConstructor constructor);
	Registrar(LevelDynamicTileID id, LevelDynamicTileConstructor constructor);
	Registrar(MapDynamicTileID id, MapDynamicTileConstructor constructor);
	Registrar(AchievementID id, AchievementConstructor constructor);
};