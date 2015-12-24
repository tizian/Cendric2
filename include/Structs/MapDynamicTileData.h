#pragma once

#include <SFML/Graphics.hpp>

#include "Enums/MapDynamicTileID.h"

struct MapDynamicTileData {
	MapDynamicTileID id;
	sf::Vector2f position;
	int spawnPosition;
	int skinNr;
};