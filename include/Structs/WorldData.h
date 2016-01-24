#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Structs/AnimatedTileData.h"
#include "Structs/LightData.h"

struct WorldData {
	std::string id;
	std::string name;
	sf::Vector2i mapSize;
	sf::Vector2i tileSize;
	std::string tileSetPath;
	std::string musicPath;
	std::vector<std::vector<int>> backgroundTileLayers;
	std::vector<std::vector<int>> lightedForegroundTileLayers;
	std::vector<std::vector<int>> foregroundTileLayers;
	
	std::vector<AnimatedTileData> animatedTiles;
	std::vector<bool> collidableTiles;
	std::vector<std::vector<bool>> collidableTilePositions;

	std::vector<LightData> lights;
	
	sf::FloatRect mapRect;
	float dimming = 0.f;
};