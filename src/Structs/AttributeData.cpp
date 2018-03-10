#include "Structs/AttributeData.h"
#include "TextProvider.h"

const std::vector<std::pair<std::string, std::function<int(const AttributeData&)>>> AttributeData::ATTR_MAP = {
	{ "Health",[](const AttributeData& attrData) {return attrData.maxHealthPoints; } },
	{ "HealthRegenerationPerS",[](const AttributeData& attrData) {return attrData.healthRegenerationPerS; } },
	{ "Haste",[](const AttributeData& attrData) {return attrData.haste; } },
	{ "Critical",[](const AttributeData& attrData) {return attrData.critical; } },
	{ "Heal",[](const AttributeData& attrData) {return attrData.heal; } },
	{ "PhysicalDamage",[](const AttributeData& attrData) {return attrData.damagePhysical; } },
	{ "FireDamage",[](const AttributeData& attrData) {return attrData.damageFire; } },
	{ "IceDamage",[](const AttributeData& attrData) {return attrData.damageIce; } },
	{ "LightDamage",[](const AttributeData& attrData) {return attrData.damageLight; } },
	{ "ShadowDamage",[](const AttributeData& attrData) {return attrData.damageShadow; } },
	{ "Armor",[](const AttributeData& attrData) {return attrData.resistancePhysical; } },
	{ "FireResistance",[](const AttributeData& attrData) {return attrData.resistanceFire; } },
	{ "IceResistance",[](const AttributeData& attrData) {return attrData.resistanceIce; } },
	{ "LightResistance",[](const AttributeData& attrData) {return attrData.resistanceLight; } },
	{ "ShadowResistance",[](const AttributeData& attrData) {return attrData.resistanceShadow; } },
};

std::string AttributeData::getAttributeText(const std::string& name, int value, bool forced) {
	if (value == 0 && !forced) return "";
	std::string s;
	if (value >= 0) {
		// these are boni on stats and should be signed
		s.append("+");
	}
	s.append(std::to_string(value));
	s.append(" ");
	s.append(g_textProvider->getText(name));
	s.append("\n");
	return s;
}

std::string AttributeData::getAttributeTextWiki(const std::string& name, int value) {
	if (value == 0) return "";
	std::string s;
	s.append("| [[" + g_textProvider->getText(name) + "]]\n");

	std::string valText;
	if (value >= 0) {
		// these are boni on stats and should be signed
		valText.append("+");
	}
	valText.append(std::to_string(value));

	s.append("| " + valText + "\n");
	return s;
}

std::string AttributeData::getItemDescriptionAttributeText(const std::string& name, int value, int& number) {
	if (value == 0) return "";
	number++;

	std::string s;
	if (value >= 0) {
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
	for (auto& it : ATTR_MAP) {
		string.append(getAttributeText(it.first, it.second(attr)));
	}
}

void AttributeData::appendItemDescriptionAttributes(std::string& string, const AttributeData& attr, int& number) {
	number = 0;
	for (auto& it : ATTR_MAP) {
		string.append(getItemDescriptionAttributeText(it.first, it.second(attr), number));
	}
}