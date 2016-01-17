#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

struct ChestData {
	int objectID;
	sf::Vector2f spawnPosition;
	int skinNr;
	int chestStrength = 0;
	std::pair<std::map<std::string, int>, int> loot;
};