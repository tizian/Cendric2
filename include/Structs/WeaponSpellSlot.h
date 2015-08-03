#pragma once

#include "global.h"

#include "Enums/SpellType.h"

struct WeaponSpellSlot
{
	SpellType type;
	bool hasStrengthModifier;
	bool hasDurationModifier;
	bool hasRangeModifier;
	bool hasSpeedModifier;
	bool hasDamageModifier;
	bool hasCountModifier;
	bool hasReflectModifier;
};

const struct WeaponSpellSlot DEFAULT_SPELLSLOT =
{
	SpellType::VOID,
	false,
	false,
	false,
	false,
	false,
	false,
	false
};