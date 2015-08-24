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
	bool needsTarget;

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

private:
	static SpellBean getChopSpellBean();

	static SpellBean getFireBallSpellBean();
	static SpellBean getIceBallSpellBean();
	// static SpellBean getTelekinesisSpellBean();
	static SpellBean getWindGustSpellBean();
	static SpellBean getAntiGravitySpellBean();
	
	// static SpellBean getUnlockSpellBean();
	// static SpellBean getShadowTrapSpellBean();
	// static SpellBean getInvisibilitySpellBean();
	// static SpellBean getFlashSpellBean();
	static SpellBean getIcyAmbushSpellBean();
	
	static SpellBean getFearSpellBean();
	static SpellBean getLeechSpellBean();
	// static SpellBean getReviveTheDeadSpellBean();
	// static SpellBean getGhostSpellBean();
	// static SpellBean getSummonCreatureSpellBean();

	// static SpellBean getLightSpellBean();
	static SpellBean getDivineShieldSpellBean();
	// static SpellBean getWrathOfTheGodsSpellBean();
	// static SpellBean getJusticeSpellBean();
	static SpellBean getAureolaSpellBean();
};

const struct SpellBean EMPTY_SPELL =
{
	SpellID::VOID,
	SpellType::VOID,
	sf::IntRect(0, 0, 0, 0),
	sf::seconds(1),
	sf::FloatRect(0, 0, 0, 0),
	DamageType::VOID,
	false,

	0,
	0,
	0,
	0.f,
	1,
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