#pragma once

#include <SFML/Graphics.hpp>

#include "Enums/MapDynamicTileID.h"

struct MapDynamicTileBean {
	MapDynamicTileID id;
	sf::Vector2f position;
	int spawnPosition;
	int skinNr;
};