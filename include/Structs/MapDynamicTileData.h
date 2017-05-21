#pragma once

#include <SFML/Graphics.hpp>
#include <map>

#include "Enums/MapDynamicTileID.h"

typedef std::map<std::string, std::string> MapTileProperties;

struct MapDynamicTileData final {
	MapDynamicTileID id;
	sf::Vector2f position;
	int skinNr;
	MapTileProperties properties;
};