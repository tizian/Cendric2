#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Structs/LightData.h"

struct ChestData {
	int objectID;
	sf::Vector2f spawnPosition;
	int skinNr;
	int chestStrength = 0;
	bool isOpen = false;
	bool isPermanent = false;
	std::string tooltipText = "";
	LightData lightData;
	std::pair<std::map<std::string, int>, int> loot;
};