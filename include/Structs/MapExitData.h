#pragma once

#include <SFML/Graphics.hpp>

struct MapExitData {
	std::string levelID;
	std::string mapID;
	sf::Vector2f spawnPoint;
	sf::FloatRect mapExitRect;
	std::set<std::string> conditions;
};