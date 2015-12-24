#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Enums/LevelDynamicTileID.h"
#include "Structs/LevelDynamicTileData.h"

struct LeverData {
	std::vector<LevelDynamicTileData> levers;
	std::vector<LevelDynamicTileData> dependentTiles;
};