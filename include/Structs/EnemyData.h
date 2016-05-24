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
	bool isUnique = false; // marker for enemies that don't respawn with the level reset when killed once
	std::string luaPath = ""; // the path to a possible lua script for behavior, mostly empty
};