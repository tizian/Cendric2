#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Structs/WorldData.h"
#include "Structs/NPCData.h"
#include "Structs/MapDynamicTileData.h"
#include "Structs/BookData.h"
#include "Structs/DoorData.h"
#include "Structs/SignData.h"
#include "Structs/ChestTileData.h"
#include "FloatTriangle.h"

struct MapData : WorldData {
	std::vector<std::pair<MapDynamicTileID, std::vector<int>>> dynamicTileLayers;
	std::vector<MapDynamicTileData> dynamicTiles;

	std::vector<NPCData> npcs;
	std::vector<BookData> books;
	std::vector<DoorData> doors;
	std::vector<SignData> signs;
	std::vector<ChestTileData> chests;
	std::vector<sf::FloatRect> collidableRects;
	std::vector<FloatTriangle> collidableTriangles;

	bool explorable = false;
};