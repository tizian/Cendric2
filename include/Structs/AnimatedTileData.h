#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

struct AnimatedTileData {
	int tileID;
	std::vector<std::pair<int, sf::Time>> frames;
};