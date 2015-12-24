#pragma once

#include "global.h"
#include "Enums/SpellType.h"

struct WeaponSpellSlotBean {
	SpellType type;
	int modifierCount;
};

const struct WeaponSpellSlotBean DEFAULT_SPELLSLOT =
{
	SpellType::VOID,
	0
};