#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Enums/DynamicTileID.h"
#include "Structs/DynamicTileBean.h"

struct LeverBean {
	std::vector<DynamicTileBean> levers;
	std::vector<DynamicTileBean> dependentTiles;
};