#pragma once

#include <SFML/Graphics.hpp>

struct JumpingTileData final {
	sf::Vector2f spawnPosition;
	int velocity; // initial velocity
	int waitingTime = 0; // in milliseconds
	int skinNr;
	int direction = 0; // in degree, default is upwards
	bool isAlternating = false; // if this is true, the jumping tile will start from its landing position again if it falls down.
	bool isAggro = false; // if this is true, the jumping tile will only jump if the main char is near enough
};