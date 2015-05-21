#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Enums/MapID.h"
#include "Enums/LevelID.h"
#include "Enums/ItemID.h"

struct CharacterCoreData
{
	std::string playerName;
	sf::Time timePlayed;
	sf::Vector2i tileSize;
	sf::Vector2f startPos;
	std::string tileSetPath;
	std::vector<std::vector<int>> layers;
	std::vector<bool> collidableTiles;
	std::vector<std::vector<bool>> collidableTileRects;
	sf::FloatRect mapRect;
};