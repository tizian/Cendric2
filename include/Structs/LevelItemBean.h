#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Enums/ResourceID.h"
#include "Enums/ItemID.h"

struct LevelItemBean
{
	sf::Vector2f spriteOffset;
	sf::FloatRect boundingBox;
	ResourceID textureID;
	std::vector<sf::IntRect> texturePositions;
	sf::Time frameTime;
	std::string tooltip;
	ItemID item;
};