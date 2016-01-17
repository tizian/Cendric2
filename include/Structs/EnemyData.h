#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Enums/EnemyID.h"

struct EnemyData {
	EnemyID id;
	int objectID;
	sf::Vector2f spawnPosition;
	std::pair<std::string, std::string> questTarget;
	std::pair<std::map<std::string, int>, int> customizedLoot;
	bool isPersistent = false; // marker for enemies that respawn with the level reset
};