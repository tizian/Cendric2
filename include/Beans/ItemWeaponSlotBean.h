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
		if (_slot_type == "elemental") {
			return SpellType::Elemental;
		} 
		if (_slot_type == "divine") {
			return SpellType::Divine;
		}
		if (_slot_type == "necromancy") {
			return SpellType::Necromancy;
		}
		if (_slot_type == "twilight") {
			return SpellType::Twilight;
		}
		if (_slot_type == "meta") {
			return SpellType::Meta;
		}
		return SpellType::VOID;
	}
};