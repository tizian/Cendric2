#pragma once

#include <SFML/Graphics.hpp>

struct MapExitData {
	std::string levelID;
	sf::Vector2f levelSpawnPoint;
	sf::FloatRect mapExitRect;
};