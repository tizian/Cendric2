#include "Structs/AttributeData.h"
#include "TextProvider.h"

inline std::string getAttributeText(const std::string& name, int value) {
	if (value == 0) return "";
	std::string s;
	s.append(g_textProvider->getText(name));
	s.append(": ");
	if (value > 0) {
		// these are boni on stats and should be signed
		s.append("+");
	}
	s.append(std::to_string(value));
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