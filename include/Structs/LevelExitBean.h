#pragma once

#include <SFML/Graphics.hpp>

struct LevelExitBean
{
	std::string mapID;
	sf::Vector2f mapSpawnPoint;
	sf::FloatRect levelExitRect;
};