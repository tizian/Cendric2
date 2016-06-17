#include "Structs/AttributeData.h"
#include "TextProvider.h"

inline std::string getAttributeText(const std::string& name, int value) {
	if (value == 0) return "";
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

inline std::string getAttributeLabelText(const std::string& name, int value) {
	if (value == 0) return "";
	std::string s;
	s.append(g_textProvider->getText(name));
	s.append(":");
	s.append("\n");
	return s;
}

inline std::string getAttributeValueText(const std::string& name, int value) {
	if (value == 0) return "";
	std::string s;
	if (value > 0) {
		// these are boni on stats and should be signed
		s.append("+");
	}
	s.append(std::to_string(value));
	s.append("\n");
	return s;
}

inline std::string getPositiveAttributeText(const std::string& name, int value, int& number) {
	if (value <= 0) return "";
	number++;

	std::string s;
	s.append("+");
	s.append(std::to_string(value));
	s.append(" ");
	s.append(g_textProvider->getText(name));
	s.append("\n");
	return s;
}

inline std::string getNegativeAttributeText(const std::string& name, int value, int& number) {
	if (value >= 0) return "";
	number++;

	std::string s;
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

void AttributeData::appendPositiveAttributes(std::string& string, const AttributeData& attr, int& number) {
	number = 0;
	string.append(getPositiveAttributeText("Health", attr.maxHealthPoints, number));
	string.append(getPositiveAttributeText("HealthRegenerationPerS", attr.healthRegenerationPerS, number));
	string.append(getPositiveAttributeText("Haste", attr.haste, number));
	string.append(getPositiveAttributeText("Critical", attr.critical, number));
	string.append(getPositiveAttributeText("Heal", attr.heal, number));
	string.append(getPositiveAttributeText("PhysicalDamage", attr.damagePhysical, number));
	string.append(getPositiveAttributeText("FireDamage", attr.damageFire, number));
	string.append(getPositiveAttributeText("IceDamage", attr.damageIce, number));
	string.append(getPositiveAttributeText("LightDamage", attr.damageLight, number));
	string.append(getPositiveAttributeText("ShadowDamage", attr.damageShadow, number));
	string.append(getPositiveAttributeText("Armor", attr.resistancePhysical, number));
	string.append(getPositiveAttributeText("FireResistance", attr.resistanceFire, number));
	string.append(getPositiveAttributeText("IceResistance", attr.resistanceIce, number));
	string.append(getPositiveAttributeText("LightResistance", attr.resistanceLight, number));
	string.append(getPositiveAttributeText("ShadowResistance", attr.resistanceShadow, number));
}

void AttributeData::appendNegativeAttributes(std::string& string, const AttributeData& attr, int& number) {
	number = 0;
	string.append(getNegativeAttributeText("Health", attr.maxHealthPoints, number));
	string.append(getNegativeAttributeText("HealthRegenerationPerS", attr.healthRegenerationPerS, number));
	string.append(getNegativeAttributeText("Haste", attr.haste, number));
	string.append(getNegativeAttributeText("Critical", attr.critical, number));
	string.append(getNegativeAttributeText("Heal", attr.heal, number));
	string.append(getNegativeAttributeText("PhysicalDamage", attr.damagePhysical, number));
	string.append(getNegativeAttributeText("FireDamage", attr.damageFire, number));
	string.append(getNegativeAttributeText("IceDamage", attr.damageIce, number));
	string.append(getNegativeAttributeText("LightDamage", attr.damageLight, number));
	string.append(getNegativeAttributeText("ShadowDamage", attr.damageShadow, number));
	string.append(getNegativeAttributeText("Armor", attr.resistancePhysical, number));
	string.append(getNegativeAttributeText("FireResistance", attr.resistanceFire, number));
	string.append(getNegativeAttributeText("IceResistance", attr.resistanceIce, number));
	string.append(getNegativeAttributeText("LightResistance", attr.resistanceLight, number));
	string.append(getNegativeAttributeText("ShadowResistance", attr.resistanceShadow, number));
}