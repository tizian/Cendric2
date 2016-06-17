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

void AttributeData::appendAttributeLabels(std::string& string, const AttributeData& attr) {
	string.append(getAttributeLabelText("Health", attr.maxHealthPoints));
	string.append(getAttributeLabelText("HealthRegenerationPerS", attr.healthRegenerationPerS));
	string.append(getAttributeLabelText("Haste", attr.haste));
	string.append(getAttributeLabelText("Critical", attr.critical));
	string.append(getAttributeLabelText("Heal", attr.heal));
	string.append(getAttributeLabelText("PhysicalDamage", attr.damagePhysical));
	string.append(getAttributeLabelText("FireDamage", attr.damageFire));
	string.append(getAttributeLabelText("IceDamage", attr.damageIce));
	string.append(getAttributeLabelText("LightDamage", attr.damageLight));
	string.append(getAttributeLabelText("ShadowDamage", attr.damageShadow));
	string.append(getAttributeLabelText("Armor", attr.resistancePhysical));
	string.append(getAttributeLabelText("FireResistance", attr.resistanceFire));
	string.append(getAttributeLabelText("IceResistance", attr.resistanceIce));
	string.append(getAttributeLabelText("LightResistance", attr.resistanceLight));
	string.append(getAttributeLabelText("ShadowResistance", attr.resistanceShadow));
}

void AttributeData::appendAttributeValues(std::string& string, const AttributeData& attr) {
	string.append(getAttributeValueText("Health", attr.maxHealthPoints));
	string.append(getAttributeValueText("HealthRegenerationPerS", attr.healthRegenerationPerS));
	string.append(getAttributeValueText("Haste", attr.haste));
	string.append(getAttributeValueText("Critical", attr.critical));
	string.append(getAttributeValueText("Heal", attr.heal));
	string.append(getAttributeValueText("PhysicalDamage", attr.damagePhysical));
	string.append(getAttributeValueText("FireDamage", attr.damageFire));
	string.append(getAttributeValueText("IceDamage", attr.damageIce));
	string.append(getAttributeValueText("LightDamage", attr.damageLight));
	string.append(getAttributeValueText("ShadowDamage", attr.damageShadow));
	string.append(getAttributeValueText("Armor", attr.resistancePhysical));
	string.append(getAttributeValueText("FireResistance", attr.resistanceFire));
	string.append(getAttributeValueText("IceResistance", attr.resistanceIce));
	string.append(getAttributeValueText("LightResistance", attr.resistanceLight));
	string.append(getAttributeValueText("ShadowResistance", attr.resistanceShadow));
}

void AttributeData::getTextureRectangles(std::vector<sf::IntRect>& rects, const AttributeData& attr) {
	if (attr.maxHealthPoints != 0) rects.push_back(sf::IntRect(0, 0, 20, 20));
	if (attr.healthRegenerationPerS != 0) rects.push_back(sf::IntRect(20, 0, 20, 20));
	if (attr.haste != 0) rects.push_back(sf::IntRect(60, 0, 20, 20));
	if (attr.critical != 0) rects.push_back(sf::IntRect(40, 0, 20, 20));
	if (attr.heal != 0) rects.push_back(sf::IntRect(0, 0, 20, 20));
	if (attr.damagePhysical != 0) rects.push_back(sf::IntRect(0, 20, 20, 20));
	if (attr.damageFire != 0) rects.push_back(sf::IntRect(20, 20, 20, 20));
	if (attr.damageIce != 0) rects.push_back(sf::IntRect(40, 20, 20, 20));
	if (attr.damageLight != 0) rects.push_back(sf::IntRect(80, 20, 20, 20));
	if (attr.damageShadow != 0) rects.push_back(sf::IntRect(60, 20, 20, 20));
	if (attr.resistancePhysical != 0) rects.push_back(sf::IntRect(0, 40, 20, 20));
	if (attr.resistanceFire != 0) rects.push_back(sf::IntRect(20, 40, 20, 20));
	if (attr.resistanceIce != 0) rects.push_back(sf::IntRect(40, 40, 20, 20));
	if (attr.resistanceLight != 0) rects.push_back(sf::IntRect(80, 40, 20, 20));
	if (attr.resistanceShadow != 0) rects.push_back(sf::IntRect(60, 40, 20, 20));
}