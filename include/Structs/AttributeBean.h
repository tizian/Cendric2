#pragma once

struct AttributeBean
{
	// total attributes
	int maxHealthPoints;
	int currentHealthPoints;
	int healthRegenerationPerS;
	int haste;
	int critical;
	int damagePhysical;
	int damageFire;
	int damageIce;
	int damageShadow;
	int damageLight;
	int resistancePhysical;
	int resistanceFire;
	int resistanceIce;
	int resistanceShadow;
	int resistanceLight;

	// calculated attributes
	float cooldownMultiplier;
	float physicalMultiplier;
	float fireMultiplier;
	float iceMultiplier;
	float shadowMultiplier;
	float lightMultiplier;
	int criticalHitChance;
};

const struct AttributeBean ZERO_ATTRIBUTES =
{
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	1.f,
	1.f,
	1.f,
	1.f,
	1.f,
	1.f,
	1
};