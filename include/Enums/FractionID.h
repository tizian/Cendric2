#pragma once

#include <iostream>

enum class FractionID {
	VOID,
	Cleric,
	Necromancer,
	Thief,
	MAX,
};

inline static FractionID resolveFractionID(const std::string& id) {
	if (id.compare("cleric") == 0) return FractionID::Cleric;
	if (id.compare("necromancer") == 0) return FractionID::Necromancer;
	if (id.compare("thief") == 0) return FractionID::Thief;
	return FractionID::VOID;
}