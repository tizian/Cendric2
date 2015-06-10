#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

struct MapData
{
	std::string name;
	sf::Vector2i mapSize;
	sf::Vector2i tileSize;
	sf::Vector2f startPos;
	std::string tileSetPath;
	std::vector<std::vector<int>> backgroundLayers;
	std::vector<std::vector<int>> foregroundLayers;
	std::vector<bool> collidableTiles;
	std::vector<std::vector<bool>> collidableTileRects;
	sf::FloatRect mapRect;
};