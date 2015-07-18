#pragma once

#include <SFML/Graphics.hpp>

#include "Enums/SpellID.h"
#include "Enums/DamageType.h"

/* describes the attributes of a spell */
struct SpellBean
{
	SpellID id;
	sf::Time cooldown;
	sf::FloatRect boundingBox;
	DamageType damageType;

	// modifiable by crystal modifiers
	int damage;
	int reflectCount;
	float startVelocity;
	int count;
	sf::Time duration;
	float range;

	float divergenceAngle;
	float rangeModifierAddition;
	int damageModifierAddition;
	int speedModifierAddition;
	int countModifierAddition;
	int reflectModifierAddition;
	sf::Time durationModifierAddition;
};

const struct SpellBean DEFAULT_CHOP =
{
	SpellID::Chop,
	sf::milliseconds(400),
	sf::FloatRect(0, 0, 40, 80),
	DamageType::Physical,

	2,
	0,
	0.f,
	1,
	sf::milliseconds(320),
	0.f,

	0.f,
	0.f,
	0,
	0,
	0,
	0,
	sf::Time::Zero
};

const struct SpellBean DEFAULT_FIREBALL =
{
	SpellID::FireBall,
	sf::milliseconds(1000),
	sf::FloatRect(0, 0, 10, 10),
	DamageType::Fire,

	10,
	0,
	300.f,
	1,
	sf::milliseconds(5000),
	0.f,

	0.2f,
	0.f,
	10,
	100,
	1,
	1,
	sf::Time::Zero
};

const struct SpellBean DEFAULT_ICEBALL =
{
	SpellID::IceBall,
	sf::milliseconds(1000),
	sf::FloatRect(0, 0, 10, 10),
	DamageType::Ice,

	6,
	0,
	300.f,
	1,
	sf::milliseconds(5000),
	0.f,

	0.2f,
	0.f,
	10,
	100,
	1,
	1,
	sf::Time::Zero
};

const struct SpellBean DEFAULT_DIVINESHIELD =
{
	SpellID::DivineShield,
	sf::milliseconds(10000),
	sf::FloatRect(0, 0, 98, 98),
	DamageType::VOID,

	0,
	0,
	0.f,
	1,
	sf::milliseconds(3000),
	0.f,

	0.f,
	0.f,
	0,
	0,
	0,
	0,
	sf::seconds(2)
};

const struct SpellBean DEFAULT_AUREOLA =
{
	SpellID::Aureola,
	sf::milliseconds(10000),
	sf::FloatRect(0, 0, 20, 20),
	DamageType::Light,

	10,
	0,
	300.f,
	4,
	sf::milliseconds(10000),
	150.f,

	2*PI_F/4,
	100.f,
	20,
	100,
	2,
	0,
	sf::Time::Zero
};
