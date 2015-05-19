#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Enums/ResourceID.h"
#include "Enums/GameObjectState.h"

struct LevelEquipmentBean
{
	sf::Vector2f spriteOffset;
	sf::FloatRect boundingBox;
	ResourceID textureID;
	std::map<GameObjectState, std::vector<sf::IntRect>> texturePositions;
	sf::Time frameTime;
};