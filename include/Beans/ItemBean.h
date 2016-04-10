#pragma once

#include "Beans/DatabaseBean.h"
#include "Enums/ItemType.h"

// an item bean, directly from the database
struct ItemBean : DatabaseBean {
	std::string item_id;
	ItemType item_type;
	sf::Vector2i icon_location;
	int gold_value;

	static ItemType resolveItemType(const std::string& _item_type) {
		if (_item_type.compare("consumable") == 0) {
			return ItemType::Consumable;
		}
		if (_item_type.compare("misc") == 0) {
			return ItemType::Misc;
		}
		if (_item_type.compare("gold") == 0) {
			return ItemType::Gold;
		}
		if (_item_type.compare("quest") == 0) {
			return ItemType::Quest;
		}
		if (_item_type.compare("document") == 0) {
			return ItemType::Document;
		}
		if (_item_type.compare("eq_weapon") == 0) {
			return ItemType::Equipment_weapon;
		}
		if (_item_type.compare("eq_head") == 0) {
			return ItemType::Equipment_head;
		}
		if (_item_type.compare("eq_neck") == 0) {
			return ItemType::Equipment_neck;
		}
		if (_item_type.compare("eq_body") == 0) {
			return ItemType::Equipment_body;
		}
		if (_item_type.compare("eq_back") == 0) {
			return ItemType::Equipment_back;
		}
		if (_item_type.compare("eq_ring") == 0) {
			return ItemType::Equipment_ring_1;
		}
		if (_item_type.compare("permanent") == 0) {
			return ItemType::Permanent;
		}
		if (_item_type.compare("convertible") == 0) {
			return ItemType::Convertible;
		}
		
		return ItemType::VOID;
	}
};