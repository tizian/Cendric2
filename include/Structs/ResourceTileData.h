#pragma once

#include <SFML/Graphics.hpp>

struct ResourceTileData {
	int objectID;
	sf::Vector2f spawnPosition;
	int skinNr;
	std::pair<std::map<std::string, int>, int> loot;
};