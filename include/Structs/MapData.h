#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Structs/WorldData.h"
#include "Structs/MapExitData.h"
#include "Structs/NPCData.h"
#include "Structs/MapDynamicTileData.h"
#include "FloatTriangle.h"

struct MapData : WorldData {
	std::vector<std::pair<MapDynamicTileID, std::vector<int>>> dynamicTileLayers;
	std::vector<MapDynamicTileData> dynamicTiles;
	
	std::vector<MapExitData> mapExits;
	std::vector<NPCData> npcs;
	std::vector<sf::FloatRect> collidableRects;
	std::vector<FloatTriangle> collidableTriangles;
};