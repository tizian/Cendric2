#pragma once

#include <iostream>

enum class FractionID {
	VOID,
	Elementalist,
	Cleric,
	Necromancer,
	Thief,
	Royalist,
	MAX,
};

inline FractionID resolveFractionID(const std::string& id) {
	if (id.compare("elementalist") == 0) return FractionID::Elementalist;
	if (id.compare("cleric") == 0) return FractionID::Cleric;
	if (id.compare("necromancer") == 0) return FractionID::Necromancer;
	if (id.compare("thief") == 0) return FractionID::Thief;
	if (id.compare("royalist") == 0) return FractionID::Royalist;
	return FractionID::VOID;
}