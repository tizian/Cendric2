#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

struct NPCBean
{
	sf::Vector2f spriteOffset;
	sf::FloatRect boundingBox;
	std::vector<sf::IntRect> texturePositions;
	sf::Time frameTime;
	std::string tooltip;
	bool talksActive;
};