#pragma once

#include "global.h"

struct LadderTileData final {
	int skinNr;
	sf::Vector2f position;
	int size = 2; // in tiles, default is 2
};