#pragma once

#include <SFML/Graphics.hpp>

#include "Enums/LevelDynamicTileID.h"

struct LevelDynamicTileData {
	LevelDynamicTileID id;
	sf::Vector2f position;
	int spawnPosition;
	int skinNr;
	// only used for FluidTile
	sf::Vector2f size;
};