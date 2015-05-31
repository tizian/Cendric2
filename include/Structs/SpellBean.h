#pragma once

#include <SFML/Graphics.hpp>

#include "Enums/SpellID.h"

/* unlike the values loaded in load() these values here are modifiable by crystal modifiers */
struct SpellBean
{
	SpellID id;
	int damage;
	int reflectCount;
	sf::Time maxActiveTime;
	sf::Time cooldown;
	float startVelocity;
	sf::FloatRect boundingBox; 
};

const struct SpellBean DEFAULT_FIRE =
{
	SpellID::Fire,
	10,
	0,
	sf::milliseconds(5000),
	sf::milliseconds(1000),
	300.f,
	sf::FloatRect(0, 0, 10, 10)
};

const struct SpellBean DEFAULT_ICE =
{
	SpellID::Ice,
	6,
	0,
	sf::milliseconds(5000),
	sf::milliseconds(1000),
	400.f,
	sf::FloatRect(0, 0, 10, 10)
};

const struct SpellBean DEFAULT_CHOP =
{
	SpellID::Chop,
	2,
	0,
	sf::milliseconds(320),
	sf::milliseconds(400),
	0.f,
	sf::FloatRect(0, 0, 40, 80)
};

const struct SpellBean DEFAULT_FORCEFIELD =
{
	SpellID::Forcefield,
	0,
	0,
	sf::milliseconds(3000),
	sf::milliseconds(10000),
	0.f,
	sf::FloatRect(0, 0, 98, 98)
};
