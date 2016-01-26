#pragma once

enum class SpellModifierType {
	VOID,
	Damage,
	Range,
	Speed,
	Reflect,
	Count,
	Strength,
	Duration,
	MAX
};

/* a simple spell modifier */
struct SpellModifier {
	SpellModifierType type;
	// level ranges from 1 to 3
	int level;

	static SpellModifierType resolveType(const std::string& name) {
		if (name.compare("damage") == 0) {
			return SpellModifierType::Damage;
		}
		else if (name.compare("duration") == 0) {
			return SpellModifierType::Duration;
		}
		else if (name.compare("count") == 0) {
			return SpellModifierType::Count;
		}
		else if (name.compare("range") == 0) {
			return SpellModifierType::Range;
		}
		else if (name.compare("reflect") == 0 || name.compare("reflection") == 0) {
			return SpellModifierType::Reflect;
		}
		else if (name.compare("speed") == 0) {
			return SpellModifierType::Speed;
		}
		else if (name.compare("strength") == 0) {
			return SpellModifierType::Strength;
		}
		else {
			return SpellModifierType::VOID;
		}
	}

	static sf::Color getSpellModifierColor(SpellModifierType type) {
		switch (type) {
		case SpellModifierType::Damage:
			return sf::Color(208 + 47, 24 + 17, 9 + 7);
		case SpellModifierType::Range:
			return sf::Color(228 + 27, 102 + 27, 0 );
		case SpellModifierType::Speed:
			return sf::Color(228 + 27, 164 + 17, 6 + 7);
		case SpellModifierType::Reflect:
			return sf::Color(118 + 24, 221 + 34, 1 );
		case SpellModifierType::Count:
			return sf::Color(0 , 173 + 82, 76 + 42);
		case SpellModifierType::Strength:
			return sf::Color(0, 86 + 43, 182 + 73);
		case SpellModifierType::Duration:
			return sf::Color(136 + 17, 0 , 228 + 27);
		default:
			// unexpected
			return sf::Color::Transparent;
		}
	}
};

const SpellModifier EMPTY_SPELLMODIFIER
{
	SpellModifierType::VOID,
	0
};