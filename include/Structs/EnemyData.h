#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Enums/EnemyID.h"

struct EnemyData final {
	EnemyID id;
	int objectID;
	int skinNr = 0;
	sf::Vector2f spawnPosition;
	std::vector<std::pair<std::string, std::string>> questTargets;
	std::pair<std::string, std::string> questCondition;
	std::pair<std::map<std::string, int>, int> customizedLoot;
	bool isUnique = false; // marker for enemies that don't respawn with the level reset when killed once
	std::string luaPath = ""; // the path to a possible lua script for behavior, mostly empty
	std::string name = ""; // if set, this enemy has not the default name, but this special one.
	bool isDead = false;
	bool isQuestRelevant = false;
};