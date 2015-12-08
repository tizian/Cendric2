#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Enums/LevelDynamicTileID.h"
#include "Structs/LevelDynamicTileBean.h"

struct LeverBean {
	std::vector<LevelDynamicTileBean> levers;
	std::vector<LevelDynamicTileBean> dependentTiles;
};