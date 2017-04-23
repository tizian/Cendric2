#pragma once

#include <SFML/Graphics.hpp>
#include "SpellModifier.h"

struct ModifierTileData final {
	sf::Vector2f spawnPosition;
	SpellModifier modifier;
};