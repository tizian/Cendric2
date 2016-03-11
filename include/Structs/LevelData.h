#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "BackgroundLayer.h"
#include "Structs/WorldData.h"
#include "Structs/LevelDynamicTileData.h"
#include "Structs/LeverData.h"
#include "Structs/EnemyData.h"
#include "Structs/ChestData.h"
#include "Structs/ModifierTileData.h"
#include "Structs/MovingTileData.h"

struct LevelData : WorldData {
	std::vector<BackgroundLayer> backgroundLayers;
	std::vector<std::pair<LevelDynamicTileID, std::vector<int>>> dynamicTileLayers;
	std::vector<LevelDynamicTileData> dynamicTiles;
	std::vector<AnimatedTileData> animatedTiles;
	std::vector<LeverData> levers;
	std::vector<std::string> levelItems;

	std::vector<EnemyData> enemies;
	std::vector<ChestData> chests;
	std::vector<ModifierTileData> modifiers;
	std::vector<MovingTileData> movingTiles;
};