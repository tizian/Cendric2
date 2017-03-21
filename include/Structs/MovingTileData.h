#pragma once

#include <SFML/Graphics.hpp>

struct MovingTileData {
	sf::Vector2f spawnPosition;
	int length = 1; // in tiles
	int distance; // in tiles
	int speed; // in pixel per s
	int skinNr;
	int initialDirection = -1; // in degree
	bool isFrozen = false;
	bool isActive = true;
	bool isOneWay = false;
	bool spikesTop = false;
	bool spikesBottom = false;
	bool isUnfreezable = false;
};