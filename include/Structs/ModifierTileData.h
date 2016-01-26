#pragma once

#include <SFML/Graphics.hpp>
#include "SpellModifier.h"

struct ModifierTileData {
	sf::Vector2f spawnPosition;
	SpellModifier modifier;
};