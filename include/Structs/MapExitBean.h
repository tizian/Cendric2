#pragma once

#include <SFML/Graphics.hpp>

#include "Enums/LevelID.h"

struct MapExitBean
{
	LevelID level;
	sf::Vector2f levelSpawnPoint;
	sf::FloatRect mapExitRect;
};