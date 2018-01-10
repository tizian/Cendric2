#pragma once

#include "Enums/SpellID.h"

// if someone asks, they are arranged according to the wavelengths of their respective color. 
enum class SpellModifierType {
	VOID,
	Count,
	Duration,
	Reflect,
	Speed,
	Range,
	Strength,
	MAX
};

/* a simple spell modifier */
struct SpellModifier final {
	SpellModifierType type = SpellModifierType::VOID;
	// level, if initialized, ranges from 1 to 3
	int level = 0;

	static SpellModifierType resolveType(const std::string& name) {
		 if (name == "duration") {
			return SpellModifierType::Duration;
		}
		else if (name == "count") {
			return SpellModifierType::Count;
		}
		else if (name == "range") {
			return SpellModifierType::Range;
		}
		else if (name == "reflect" || name == "reflection") {
			return SpellModifierType::Reflect;
		}
		else if (name == "speed") {
			return SpellModifierType::Speed;
		}
		else if (name == "strength") {
			return SpellModifierType::Strength;
		}
		else {
			return SpellModifierType::VOID;
		}
	}

	static sf::Color getSpellModifierColor(SpellModifierType type) {
		switch (type) {
		case SpellModifierType::Range:
			return sf::Color(228 + 27, 102 + 27, 0 ); // orange
		case SpellModifierType::Speed:
			return sf::Color(228 + 27, 164 + 17, 6 + 7); // yellow
		case SpellModifierType::Reflect:
			return sf::Color(118 + 24, 221 + 34, 1 ); // green
		case SpellModifierType::Count:
			return sf::Color(136 + 17, 0, 228 + 27); // purple
		case SpellModifierType::Strength:
			return sf::Color(208 + 47, 24 + 17, 9 + 7); // red
		case SpellModifierType::Duration:
			return sf::Color(0, 86 + 43, 182 + 73); // blue
		default:
			// unexpected
			return COLOR_TRANSPARENT;
		}
	}
};

typedef std::pair<SpellID, std::vector<SpellModifier>> WeaponSlot;