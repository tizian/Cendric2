#pragma once

#include "Beans/DatabaseBean.h"
#include "Enums/ItemType.h"

// an item bean, directly from the database
struct ItemBean final : DatabaseBean {
	std::string item_id;
	ItemType item_type;
	sf::Vector2i icon_location;
	int gold_value;
	int rarity;

	static ItemType resolveItemType(const std::string& _item_type) {
		if (_item_type == "consumable") {
			return ItemType::Consumable;
		}
		if (_item_type == "misc") {
			return ItemType::Misc;
		}
		if (_item_type == "gold") {
			return ItemType::Gold;
		}
		if (_item_type == "quest") {
			return ItemType::Quest;
		}
		if (_item_type == "document") {
			return ItemType::Document;
		}
		if (_item_type == "eq_weapon") {
			return ItemType::Equipment_weapon;
		}
		if (_item_type == "eq_head") {
			return ItemType::Equipment_head;
		}
		if (_item_type == "eq_neck") {
			return ItemType::Equipment_neck;
		}
		if (_item_type == "eq_body") {
			return ItemType::Equipment_body;
		}
		if (_item_type == "eq_back") {
			return ItemType::Equipment_back;
		}
		if (_item_type == "eq_ring") {
			return ItemType::Equipment_ring_1;
		}
		if (_item_type == "permanent") {
			return ItemType::Permanent;
		}
		if (_item_type == "convertible") {
			return ItemType::Convertible;
		}
		if (_item_type == "spell") {
			return ItemType::Spell;
		}
		if (_item_type == "key") {
			return ItemType::Key;
		}
		
		return ItemType::VOID;
	}
};