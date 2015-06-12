#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Enums/LevelID.h>

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
	std::vector<std::pair<sf::FloatRect, LevelID>> levelEntries;
	sf::FloatRect mapRect;
};