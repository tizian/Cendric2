#pragma once

#include <SFML/Graphics.hpp>

struct MapExitBean
{
	std::string levelID;
	sf::Vector2f levelSpawnPoint;
	sf::FloatRect mapExitRect;
};