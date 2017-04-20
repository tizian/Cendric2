#include "Structs/AttributeData.h"
#include "TextProvider.h"

std::string AttributeData::getAttributeText(const std::string& name, int value, bool forced) {
	if (value == 0 && !forced) return "";
	std::string s;
	if (value > 0) {
		// these are boni on stats and should be signed
		s.append("+");
	}
	s.append(std::to_string(value));
	s.append(" ");
	s.append(g_textProvider->getText(name));
	s.append("\n");
	return s;
}

std::string AttributeData::getItemDescriptionAttributeText(const std::string& name, int value, int& number) {
	if (value == 0) return "";
	number++;

	std::string s;
	if (value > 0) {
		// these are boni on stats and should be signed
		s.append("+");
	}
	s.append(std::to_string(value));
	s.append(" ");
	s.append(g_textProvider->getText(name));
	s.append("\n");
	return s;
}

void AttributeData::appendAttributes(std::string& string, const AttributeData& attr) {
	string.append(getAttributeText("Health", attr.maxHealthPoints));
	string.append(getAttributeText("HealthRegenerationPerS", attr.healthRegenerationPerS));
	string.append(getAttributeText("Haste", attr.haste));
	string.append(getAttributeText("Critical", attr.critical));
	string.append(getAttributeText("Heal", attr.heal));
	string.append(getAttributeText("PhysicalDamage", attr.damagePhysical));
	string.append(getAttributeText("FireDamage", attr.damageFire));
	string.append(getAttributeText("IceDamage", attr.damageIce));
	string.append(getAttributeText("LightDamage", attr.damageLight));
	string.append(getAttributeText("ShadowDamage", attr.damageShadow));
	string.append(getAttributeText("Armor", attr.resistancePhysical));
	string.append(getAttributeText("FireResistance", attr.resistanceFire));
	string.append(getAttributeText("IceResistance", attr.resistanceIce));
	string.append(getAttributeText("LightResistance", attr.resistanceLight));
	string.append(getAttributeText("ShadowResistance", attr.resistanceShadow));
}

void AttributeData::appendItemDescriptionAttributes(std::string& string, const AttributeData& attr, int& number) {
	number = 0;
	string.append(getItemDescriptionAttributeText("Health", attr.maxHealthPoints, number));
	string.append(getItemDescriptionAttributeText("HealthRegenerationPerS", attr.healthRegenerationPerS, number));
	string.append(getItemDescriptionAttributeText("Haste", attr.haste, number));
	string.append(getItemDescriptionAttributeText("Critical", attr.critical, number));
	string.append(getItemDescriptionAttributeText("Heal", attr.heal, number));
	string.append(getItemDescriptionAttributeText("PhysicalDamage", attr.damagePhysical, number));
	string.append(getItemDescriptionAttributeText("FireDamage", attr.damageFire, number));
	string.append(getItemDescriptionAttributeText("IceDamage", attr.damageIce, number));
	string.append(getItemDescriptionAttributeText("LightDamage", attr.damageLight, number));
	string.append(getItemDescriptionAttributeText("ShadowDamage", attr.damageShadow, number));
	string.append(getItemDescriptionAttributeText("Armor", attr.resistancePhysical, number));
	string.append(getItemDescriptionAttributeText("FireResistance", attr.resistanceFire, number));
	string.append(getItemDescriptionAttributeText("IceResistance", attr.resistanceIce, number));
	string.append(getItemDescriptionAttributeText("LightResistance", attr.resistanceLight, number));
	string.append(getItemDescriptionAttributeText("ShadowResistance", attr.resistanceShadow, number));
}