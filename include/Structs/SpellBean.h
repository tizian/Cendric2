#pragma once

#include <SFML/Graphics.hpp>
#include "global.h"

#include "Enums/SpellID.h"
#include "Enums/SpellType.h"
#include "Enums/DamageType.h"
#include "Enums/Key.h"

/* describes the attributes of a spell */
struct SpellBean
{
	SpellID id;
	SpellType spellType;
	sf::IntRect iconTextureRect;
	sf::Time cooldown;
	sf::FloatRect boundingBox;
	DamageType damageType;

	// modifiable by crystal modifiers
	int damage;
	int heal;
	int reflectCount;
	float startVelocity;
	int count;
	sf::Time duration;
	float range;

	float divergenceAngle;
	float rangeModifierAddition;
	int damageModifierAddition;
	float speedModifierAddition;
	int countModifierAddition;
	int reflectModifierAddition;
	sf::Time durationModifierAddition;

	Key inputKey;

	static SpellBean getSpellBean(SpellID id);
};

const struct SpellBean EMPTY_SPELL =
{
	SpellID::VOID,
	SpellType::VOID,
	sf::IntRect(0, 0, 0, 0),
	sf::seconds(1),
	sf::FloatRect(0, 0, 0, 0),
	DamageType::VOID,

	0,
	0,
	0,
	0.f,
	0,
	sf::Time::Zero,
	0.f,

	0.f,
	0.f,
	0,
	0,
	0,
	0,
	sf::Time::Zero,

	Key::VOID
};

const struct SpellBean DEFAULT_CHOP =
{
	SpellID::Chop,
	SpellType::VOID,
	sf::IntRect(0, 0, 0, 0),
	sf::milliseconds(400),
	sf::FloatRect(0, 0, 40, 80),
	DamageType::Physical,

	2,
	0,
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
	sf::Time::Zero,

	Key::Chop
};

const struct SpellBean DEFAULT_FIREBALL =
{
	SpellID::FireBall,
	SpellType::Elemental,
	sf::IntRect(0, 0, 50, 50),
	sf::milliseconds(1000),
	sf::FloatRect(0, 0, 10, 10),
	DamageType::Fire,

	10,
	0,
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
	sf::Time::Zero,

	Key::VOID
};

const struct SpellBean DEFAULT_ICEBALL =
{
	SpellID::IceBall,
	SpellType::Elemental,
	sf::IntRect(50, 0, 50, 50),
	sf::milliseconds(1000),
	sf::FloatRect(0, 0, 10, 10),
	DamageType::Ice,

	6,
	0,
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
	sf::Time::Zero,

	Key::VOID
};

const struct SpellBean DEFAULT_DIVINESHIELD =
{
	SpellID::DivineShield,
	SpellType::Divine,
	sf::IntRect(100, 150, 50, 50),
	sf::milliseconds(10000),
	sf::FloatRect(0, 0, 98, 98),
	DamageType::VOID,

	0,
	20,
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
	SpellType::Divine,
	sf::IntRect(200, 150, 50, 50),
	sf::milliseconds(10000),
	sf::FloatRect(0, 0, 20, 20),
	DamageType::Light,

	10,
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
	sf::Time::Zero,

	Key::VOID
};

const struct SpellBean DEFAULT_FEAR =
{
	SpellID::Fear,
	SpellType::Necromancy,
	sf::IntRect(0, 100, 50, 50),
	sf::milliseconds(3000),
	sf::FloatRect(0, 0, 10, 10),
	DamageType::VOID,

	0,
	0,
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
	sf::seconds(2),

	Key::VOID
};

const struct SpellBean DEFAULT_ANTIGRAVITY =
{
	SpellID::AntiGravity,
	SpellType::Elemental,
	sf::IntRect(200, 0, 50, 50),
	sf::milliseconds(20000),
	sf::FloatRect(0, 0, 98, 98),
	DamageType::VOID,

	0,
	0,
	0,
	0.f,
	1,
	sf::milliseconds(5000),
	0.f,

	0.f,
	0.f,
	0,
	0,
	0,
	0,
	sf::seconds(3)
};

const struct SpellBean DEFAULT_WINDGUST =
{
	SpellID::WindGust,
	SpellType::Elemental,
	sf::IntRect(150, 0, 50, 50),
	sf::milliseconds(3000),
	sf::FloatRect(0, 0, 60, 40),
	DamageType::VOID,

	0,
	0,
	0,
	0.f,
	1,
	sf::seconds(1),
	20.f,

	0.f,
	0.f,
	0,
	0,
	0,
	0,
	sf::seconds(1)
};

const struct SpellBean DEFAULT_LEECH =
{
	SpellID::Leech,
	SpellType::Necromancy,
	sf::IntRect(50, 100, 50, 50),
	sf::milliseconds(3000),
	sf::FloatRect(0, 0, 10, 10),
	DamageType::Shadow,

	10,
	0,
	0,
	100.f,
	1,
	sf::milliseconds(5000),
	0.f,

	0.2f,
	0.f,
	10,
	50.f,
	1,
	1,
	sf::Time::Zero,

	Key::VOID
};
