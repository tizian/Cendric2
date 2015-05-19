#pragma once

#include <SFML/Graphics.hpp>

#include "Enums/SpellID.h"

/* unlike the values loaded in load() these values here are modifiable by crystal modifiers */
struct SpellBean
{
	SpellID id;
	int damage;
	int reflectCount;
	sf::Time cooldown;
	sf::Time maxActiveTime;
	float startVelocity;
	sf::FloatRect boundingBox; // gets only set after load if it is not [0,0,0,0]
};