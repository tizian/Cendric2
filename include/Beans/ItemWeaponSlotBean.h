#pragma once

#include <iostream>
#include "Beans/DatabaseBean.h"
#include "Enums/SpellType.h"

// an item weapon slot bean, directly from the database
struct ItemWeaponSlotBean final : DatabaseBean {
	std::string item_id;
	int slot_nr;
	SpellType slot_type;
	int modifier_count;

	static SpellType resolveSlotType(const std::string& _slot_type) {
		if (_slot_type.compare("elemental") == 0) {
			return SpellType::Elemental;
		} 
		if (_slot_type.compare("divine") == 0) {
			return SpellType::Divine;
		}
		if (_slot_type.compare("necromancy") == 0) {
			return SpellType::Necromancy;
		}
		if (_slot_type.compare("twilight") == 0) {
			return SpellType::Twilight;
		}
		if (_slot_type.compare("meta") == 0) {
			return SpellType::Meta;
		}
		return SpellType::VOID;
	}
};