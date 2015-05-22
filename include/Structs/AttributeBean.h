#pragma once

struct AttributeBean
{
	int maxHealthPoints;
	int maxManaPoints;
	int healthRegenerationPerS;
	int manaRegenerationPerS;
	int damagePhysical;
	int damageFire;
	int damageIce;
	int resistancePhysical;
	int resistanceFire;
	int resistanceIce;
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
	0
};