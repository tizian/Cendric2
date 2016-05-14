#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Structs/LevelDynamicTileData.h"
#include "Structs/MovingTileData.h"

// be aware that levers could handle all of these dependent tiles at once
// but the level format does not (yet?) allow it.
struct LeverData {
	std::vector<LevelDynamicTileData> levers;
	std::vector<LevelDynamicTileData> dependentSwitchableTiles;
	std::vector<MovingTileData> dependentMovingTiles;
};