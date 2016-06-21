#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Enums/EnemyID.h"

struct EnemyData {
	EnemyID id;
	int objectID;
	sf::Vector2f spawnPosition;
	std::vector<std::pair<std::string, std::string>> questTargets;
	std::pair<std::string, std::string> questCondition;
	std::pair<std::map<std::string, int>, int> customizedLoot;
	bool isUnique = false; // marker for enemies that don't respawn with the level reset when killed once
	bool isBoss = false; // marker for boss enemies. killing a boss will set the condition "boss", "<enemy name>" 
						 // the boss property also implies the unique property.
	std::string luaPath = ""; // the path to a possible lua script for behavior, mostly empty
};