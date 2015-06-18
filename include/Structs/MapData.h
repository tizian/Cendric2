#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Enums/LevelID.h>
#include <Structs/MapExitBean.h>

struct MapData
{
	std::string name;
	sf::Vector2i mapSize;
	sf::Vector2i tileSize;
	std::string tileSetPath;
	std::vector<std::vector<int>> backgroundLayers;
	std::vector<std::vector<int>> foregroundLayers;
	std::vector<bool> collidableTiles;
	std::vector<std::vector<bool>> collidableTileRects;
	std::vector<MapExitBean> levelEntries;
	sf::FloatRect mapRect;
};