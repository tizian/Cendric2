#pragma once

#include <SFML/Graphics.hpp>
#include <map>

#include "Enums/LevelDynamicTileID.h"

typedef std::map<std::string, std::string> LevelTileProperties;

struct LevelDynamicTileData final {
	LevelDynamicTileID id = LevelDynamicTileID::VOID;
	sf::Vector2f position;
	int objectID = -1;
	int skinNr = 0;
	std::map<std::string, std::string> properties;
};