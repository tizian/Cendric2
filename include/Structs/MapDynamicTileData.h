#pragma once

#include <SFML/Graphics.hpp>

#include "Enums/MapDynamicTileID.h"

struct MapDynamicTileData {
	MapDynamicTileID id;
	int spawnPosition;
	int skinNr;
};