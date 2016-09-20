#pragma once

#include "global.h"
#include "Structs/Condition.h"

struct DoorData {
	int skinNr;
	sf::Vector2f position;
	std::string keyItemID = "";
	std::vector<Condition> conditions;
};