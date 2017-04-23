#pragma once

#include <SFML/Graphics.hpp>

#include "Enums/MapDynamicTileID.h"

struct MapDynamicTileData final {
	MapDynamicTileID id;
	int spawnPosition;
	int skinNr;
};