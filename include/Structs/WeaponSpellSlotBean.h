#pragma once

#include <set>

#include "global.h"
#include "Enums/SpellType.h"
#include "Structs/SpellModifier.h"

struct WeaponSpellSlotBean
{
	SpellType type;
	std::set<SpellModifierType> allowedModifiers;
};

const struct WeaponSpellSlotBean DEFAULT_SPELLSLOT =
{
	SpellType::VOID,
	std::set<SpellModifierType>()
};