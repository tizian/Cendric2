#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "World/BackgroundLayer.h"
#include "Structs/WorldData.h"
#include "Structs/LevelDynamicTileData.h"
#include "Structs/LeverData.h"
#include "Structs/EnemyData.h"

struct LevelData final : WorldData {
	std::vector<BackgroundLayer> backgroundLayers;
	std::vector<LevelDynamicTileData> dynamicTiles;
	std::vector<AnimatedTileData> animatedTiles;
	std::vector<LeverData> levers;
	std::vector<std::string> levelItems;

	std::vector<EnemyData> enemies;

	std::string bossLevelPath;
	bool isBossLevel = false;
	bool isObserved = false;
};