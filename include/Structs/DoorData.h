#pragma once

#include "global.h"
#include "Structs/Condition.h"

struct DoorData final {
	int skinNr;
	sf::Vector2f position;
	std::string keyItemID = "";
	std::vector<Condition> conditions;
	bool isCollidable = false;
	int tileWidth = 1;
	int strength = 0;
};