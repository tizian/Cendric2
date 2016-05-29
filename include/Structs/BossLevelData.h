#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

struct BossLevelData {
	bool isBossLevel = false;

	bool isInLevel;
	std::string currentWorld;
	sf::Vector2f currentWorldPosition;
};