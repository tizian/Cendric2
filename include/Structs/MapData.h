#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Structs/TMXData.h"
#include "Structs/MapExitBean.h"
#include "Structs/NPCBean.h"
#include "Structs/MapDynamicTileBean.h"

struct MapData : TMXData {
	std::vector<std::pair<MapDynamicTileID, std::vector<int>>> dynamicTileLayers;
	std::vector<MapDynamicTileBean> dynamicTiles;
	
	std::vector<MapExitBean> mapExits;
	std::vector<NPCBean> npcs;
};