#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Structs/MapExitBean.h"

struct MapData
{
	std::string id;
	std::string name;
	sf::Vector2i mapSize;
	sf::Vector2i tileSize;
	std::string tileSetPath;
	std::vector<std::vector<int>> backgroundLayers;
	std::vector<std::vector<int>> foregroundLayers;
	std::vector<bool> collidableTiles;
	std::vector<std::vector<bool>> collidableTileRects;
	std::vector<MapExitBean> mapExits;
	std::vector<std::string> npcs;
	std::vector<std::pair<std::string, sf::Vector2f>> npcPositions;
	sf::FloatRect mapRect;
};