#pragma once

#include "global.h"
#include "Enums/SpellType.h"

struct WeaponSpellSlotData final {
	SpellType type = SpellType::VOID;
	int modifierCount = 0;
};
