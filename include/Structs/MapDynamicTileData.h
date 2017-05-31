#pragma once

#include <SFML/Graphics.hpp>
#include <map>

#include "Enums/MapDynamicTileID.h"

typedef std::map<std::string, std::string> MapTileProperties;

struct MapDynamicTileData final {
	MapDynamicTileID id = MapDynamicTileID::VOID;
	sf::Vector2f position;
	int skinNr = 0;
	int objectID = -1;
	MapTileProperties properties;
};