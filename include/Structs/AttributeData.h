#pragma once

#include "Beans/ItemAttributeBean.h"

struct AttributeData {
	int currentHealthPoints;

	// total attributes
	int maxHealthPoints;
	int healthRegenerationPerS;
	int haste;
	int critical;
	int heal;
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
	float calculateDamageReduction(int resistance) const {
		if (resistance >= 0) {
			return 100.f / (100.f + resistance);
		}
		return 2.f - 100.f / (100.f - resistance);
	}

	// calculates the data from the database bean
	void create(const ItemAttributeBean& bean) {
		if (bean.status != BeanStatus::Filled) {
			return;
		}

		maxHealthPoints = bean.max_health;
		healthRegenerationPerS = bean.health_regeneration;
		haste = bean.haste;
		critical = bean.critical;
		heal = bean.heal;
		damagePhysical = bean.dmg_physical;
		damageFire = bean.dmg_fire;
		damageIce = bean.dmg_ice;
		damageShadow = bean.dmg_shadow;
		damageLight = bean.dmg_light;
		resistancePhysical = bean.res_physical;
		resistanceFire = bean.res_fire;
		resistanceIce = bean.res_ice;
		resistanceShadow = bean.res_shadow;
		resistanceLight = bean.res_light;

		calculateAttributes();
	}

	// calculates calculated attributes based on total attributes
	void calculateAttributes() {
		criticalHitChance = std::max(0, std::min(60, critical));
		cooldownMultiplier = 1.f / (1.f + (haste / 100.f));

		physicalMultiplier = calculateDamageReduction(resistancePhysical);
		fireMultiplier = calculateDamageReduction(resistanceFire);
		iceMultiplier = calculateDamageReduction(resistanceIce);
		shadowMultiplier = calculateDamageReduction(resistanceShadow);
		lightMultiplier = calculateDamageReduction(resistanceLight);
	}

	// adds the second bean to the this bean. current health points will be added.
	void addBean(const AttributeData& secondBean) {
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
		heal += secondBean.heal;
		healthRegenerationPerS += secondBean.healthRegenerationPerS;
		currentHealthPoints = (maxHealthPoints < currentHealthPoints + secondBean.currentHealthPoints) ? maxHealthPoints : (currentHealthPoints + secondBean.currentHealthPoints);

		calculateAttributes();
	}

	// removes the second bean from the this bean. current health points will not be removed!
	void removeBean(const AttributeData& secondBean) {
		damageFire -= secondBean.damageFire;
		damageIce -= secondBean.damageIce;
		damagePhysical -= secondBean.damagePhysical;
		damageLight -= secondBean.damageLight;
		damageShadow -= secondBean.damageShadow;
		resistanceFire -= secondBean.resistanceFire;
		resistanceIce -= secondBean.resistanceIce;
		resistancePhysical -= secondBean.resistancePhysical;
		resistanceLight -= secondBean.resistanceLight;
		resistanceShadow -= secondBean.resistanceShadow;
		maxHealthPoints -= secondBean.maxHealthPoints;
		haste -= secondBean.haste;
		critical -= secondBean.critical;
		heal -= secondBean.heal;
		healthRegenerationPerS -= secondBean.healthRegenerationPerS;
		currentHealthPoints = std::min(currentHealthPoints, maxHealthPoints);

		calculateAttributes();
	}

	// sets the current health points and the max points to the health value
	void setHealth(int health) {
		maxHealthPoints = health;
		currentHealthPoints = health;
	}

	// appends the attributes to the string, seperated with \n and prefixed with + or -
	static void appendAttributes(std::string& string, const AttributeData& attr);
	static void appendItemDescriptionAttributes(std::string& string, const AttributeData& attr, int& number);
};

const struct AttributeData ZERO_ATTRIBUTES =
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
	0,
	1.f,
	1.f,
	1.f,
	1.f,
	1.f,
	1.f,
	0
};