#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "BackgroundLayer.h"
#include "Structs/WorldData.h"
#include "Structs/LevelDynamicTileData.h"
#include "Structs/LeverData.h"
#include "Structs/EnemyData.h"
#include "Structs/ChestTileData.h"
#include "Structs/ModifierTileData.h"
#include "Structs/MovingTileData.h"
#include "Structs/JumpingTileData.h"
#include "Structs/SignData.h"
#include "Structs/LadderTileData.h"
#include "Structs/DoorData.h"

struct LevelData : WorldData {
	std::vector<BackgroundLayer> backgroundLayers;
	std::vector<LevelDynamicTileData> dynamicTiles;
	std::vector<AnimatedTileData> animatedTiles;
	std::vector<LeverData> levers;
	std::vector<std::string> levelItems;

	std::vector<EnemyData> enemies;
	std::vector<ChestTileData> chests;
	std::vector<ModifierTileData> modifiers;
	std::vector<JumpingTileData> jumpingTiles;
	std::vector<SignData> signTiles;
	std::vector<LadderTileData> ladderTiles;
	std::vector<DoorData> doors;

	bool isBossLevel = false;
	bool isObserved = false;
};