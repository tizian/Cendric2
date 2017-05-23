#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Structs/LevelDynamicTileData.h"

// be aware that levers could handle all of these dependent tiles at once
// but the level format does not (yet?) allow it.
struct LeverData final {
	std::vector<LevelDynamicTileData> levers;
	std::vector<LevelDynamicTileData> dependentSwitchableTiles;
	std::vector<LevelDynamicTileData> dependentMovingTiles;
};