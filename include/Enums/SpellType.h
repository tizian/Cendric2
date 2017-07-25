#pragma once

enum class SpellType {
	VOID,

	Elemental,
	Twilight,
	Necromancy,
	Divine,
	Meta,

	MAX
};

static sf::Color getColorFromSpellType(SpellType type) {
	switch (type) {
	case SpellType::Elemental:
		return COLOR_ELEMENTAL;
	case SpellType::Twilight:
		return COLOR_TWILIGHT;
	case SpellType::Necromancy:
		return COLOR_NECROMANCY;
	case SpellType::Divine:
		return COLOR_DIVINE;
	default:
		return COLOR_WHITE;
	}
}

