#pragma once

#include <SFML/Graphics.hpp>

struct LevelExitData {
	std::string mapID;
	std::string levelID;
	sf::Vector2f spawnPoint;
	sf::FloatRect levelExitRect;
};