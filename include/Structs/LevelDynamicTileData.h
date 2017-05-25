#pragma once

#include <SFML/Graphics.hpp>
#include <map>

#include "Enums/LevelDynamicTileID.h"

typedef std::map<std::string, std::string> LevelTileProperties;

struct LevelDynamicTileData final {
	LevelDynamicTileID id;
	sf::Vector2f position;
	int objectID;
	int skinNr;
	std::map<std::string, std::string> properties;
};