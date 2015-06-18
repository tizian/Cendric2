#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Enums/ResourceID.h"
#include "Enums/ItemID.h"

struct LevelItemBean
{
	sf::Vector2f spriteOffset;
	sf::FloatRect boundingBox;
	std::vector<sf::IntRect> texturePositions;
	sf::Time frameTime;
	std::string tooltip;
	ItemID item;
	// only relevant for gold items. They will have their item id void.
	int goldValue;
};