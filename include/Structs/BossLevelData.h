#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

struct BossLevelData {
	bool isBossLevel = false;

	bool isOnWinLevel;
	std::string onWinWorld;
	sf::Vector2f onWinPosition;

	bool isOnLoseLevel;
	std::string onLoseWorld;
	sf::Vector2f onLosePosition;
};