#pragma once

struct AttributeBean
{
	int currentHealthPoints;

	// total attributes
	int maxHealthPoints;
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

	// helper function
	float calculateDamageReduction(int resistance) const
	{
		if (resistance >= 0)
		{
			return 100.f / (100.f + resistance);
		}
		return 2.f - 100.f / (100.f - resistance);
	}

	// calculates calculated attributes based on total attributes
	void calculateAttributes()
	{
		criticalHitChance = std::max(0, std::min(60, critical));
		cooldownMultiplier = 1.f / (1.f + (haste / 100.f));

		physicalMultiplier = calculateDamageReduction(resistancePhysical);
		fireMultiplier = calculateDamageReduction(resistanceFire);
		iceMultiplier = calculateDamageReduction(resistanceIce);
		shadowMultiplier = calculateDamageReduction(resistanceShadow);
		lightMultiplier = calculateDamageReduction(resistanceLight);
	}

	// adds the second bean to the this bean
	void addBean(const AttributeBean& secondBean)
	{
		damageFire += secondBean.damageFire;
		damageIce += secondBean.damageIce;
		damagePhysical += secondBean.damagePhysical;
		damageLight += secondBean.damageLight;
		damageShadow += secondBean.damageShadow;
		resistanceFire += secondBean.resistanceFire;
		resistanceIce += secondBean.resistanceIce;
		resistancePhysical += secondBean.resistancePhysical;
		resistanceLight += secondBean.resistanceLight;
		resistanceShadow += secondBean.resistanceShadow;
		maxHealthPoints += secondBean.maxHealthPoints;
		haste += secondBean.haste;
		critical += secondBean.critical;
		healthRegenerationPerS += secondBean.healthRegenerationPerS;
		currentHealthPoints = (maxHealthPoints < currentHealthPoints + secondBean.currentHealthPoints) ? maxHealthPoints : (currentHealthPoints + secondBean.currentHealthPoints);
	}
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
	0
};