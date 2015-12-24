#pragma once

#include <SFML/Graphics.hpp>

struct LevelExitData {
	std::string mapID;
	sf::Vector2f mapSpawnPoint;
	sf::FloatRect levelExitRect;
};