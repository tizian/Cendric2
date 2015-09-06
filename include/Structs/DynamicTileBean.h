#pragma once

#include <SFML/Graphics.hpp>

#include "Enums/DynamicTileID.h"

struct DynamicTileBean
{
	DynamicTileID id;
	sf::Vector2f position;
	int spawnPosition;
	int skinNr;
	// only used for SWE Simulated Water Tiles
	sf::Vector2f size;
};