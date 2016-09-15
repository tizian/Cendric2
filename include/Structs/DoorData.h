#pragma once

#include "global.h"

struct Condition {
	bool negative;
	std::string type;
	std::string name;
};

struct DoorData {
	int skinNr;
	sf::Vector2f position;
	std::string keyItemID = "";
	std::vector<Condition> conditions;
};