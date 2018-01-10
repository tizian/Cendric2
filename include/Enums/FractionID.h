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
	if (id == "cleric") return FractionID::Cleric;
	if (id == "necromancer") return FractionID::Necromancer;
	if (id == "thief") return FractionID::Thief;
	return FractionID::VOID;
}