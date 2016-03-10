#pragma once

#include <SFML/Graphics.hpp>

#include "Structs/TriggerContent.h"

struct TriggerData {
	std::string worldID;
	sf::FloatRect triggerRect;
	int objectID;
	bool isPersistent = false;

	std::vector<TriggerContent> content;
};