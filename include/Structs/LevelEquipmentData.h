#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Enums/GameObjectState.h"

struct LevelEquipmentData final {
	sf::Vector2f spriteOffset;
	sf::FloatRect boundingBox;
	std::string texturePath;
	std::map<GameObjectState, std::vector<sf::IntRect>> texturePositions;
};