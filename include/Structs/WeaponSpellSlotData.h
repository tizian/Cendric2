#pragma once

#include "global.h"
#include "Enums/SpellType.h"

struct WeaponSpellSlotData {
	SpellType type;
	int modifierCount;
};

const struct WeaponSpellSlotData DEFAULT_SPELLSLOT =
{
	SpellType::VOID,
	0
};