#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Structs/MapExitBean.h"
#include "Structs/NPCBean.h"
#include "Structs/LightBean.h"

struct MapData {
	std::string id;
	std::string name;
	sf::Vector2i mapSize;
	sf::Vector2i tileSize;
	std::string tileSetPath;
	std::string musicPath;
	std::vector<std::vector<int>> backgroundLayers;
	std::vector<std::vector<int>> foregroundLayers;
	std::vector<bool> collidableTiles;
	std::vector<std::vector<bool>> collidableTileRects;
	std::vector<MapExitBean> mapExits;
	std::vector<NPCBean> npcs;
	std::vector<LightBean> lights;
	sf::FloatRect mapRect;
	float dimming = 0.f;
};