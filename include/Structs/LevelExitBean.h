#pragma once

#include <SFML/Graphics.hpp>

#include "Enums/MapID.h"

struct LevelExitBean
{
	MapID map;
	sf::Vector2f mapSpawnPoint;
	sf::FloatRect levelExitRect;
};