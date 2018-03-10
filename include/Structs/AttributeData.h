#pragma once

#include "Beans/ItemAttributeBean.h"
#include "global.h"

struct AttributeData final {
	int currentHealthPoints = 0;

	// total attributes
	int maxHealthPoints = 0;
	int healthRegenerationPerS = 0;
	int haste = 0;
	int critical = 0;
	int heal = 0;
	int damagePhysical = 0;
	int damageFire = 0;
	int damageIce = 0;
	int damageShadow = 0;
	int damageLight = 0;
	int resistancePhysical = 0;
	int resistanceFire = 0;
	int resistanceIce = 0;
	int resistanceShadow = 0;
	int resistanceLight = 0;

	// calculated attributes
	float cooldownMultiplier = 1.f;
	float physicalMultiplier = 1.f;
	float fireMultiplier = 1.f;
	float iceMultiplier = 1.f;
	float shadowMultiplier = 1.f;
	float lightMultiplier = 1.f;
	int criticalHitChance = 0;

	// helper function
	float calculateDamageReduction(int resistance) const {
		if (resistance >= 0) {
			return 100.f / (100.f + resistance);
		}
		return 2.f - 100.f / (100.f - resistance);
	}

	// calculates the data from the database bean
	void create(const ItemAttributeBean* bean) {
		if (bean == nullptr) {
			return;
		}

		maxHealthPoints = bean->max_health;
		healthRegenerationPerS = bean->health_regeneration;
		haste = bean->haste;
		critical = bean->critical;
		heal = bean->heal;
		damagePhysical = bean->dmg_physical;
		damageFire = bean->dmg_fire;
		damageIce = bean->dmg_ice;
		damageShadow = bean->dmg_shadow;
		damageLight = bean->dmg_light;
		resistancePhysical = bean->res_physical;
		resistanceFire = bean->res_fire;
		resistanceIce = bean->res_ice;
		resistanceShadow = bean->res_shadow;
		resistanceLight = bean->res_light;

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
	static std::string getAttributeText(const std::string& name, int value, bool forced = false);
	static std::string getAttributeTextWiki(const std::string& name, int value);
	static std::string getItemDescriptionAttributeText(const std::string& name, int value, int& number);
	static void appendAttributes(std::string& string, const AttributeData& attr);
	static void appendItemDescriptionAttributes(std::string& string, const AttributeData& attr, int& number);

	static const std::vector<std::pair<std::string, std::function<int(const AttributeData&)>>> ATTR_MAP;
};