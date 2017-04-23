#pragma once

#include "global.h"

struct SignData final {
	int skinNr;
	sf::Vector2f position;
	std::string text;
	bool isHint = false;
};