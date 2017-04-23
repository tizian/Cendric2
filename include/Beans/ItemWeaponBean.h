#pragma once

#include <iostream>
#include "Beans/DatabaseBean.h"

// an item weapon bean, directly from the database
struct ItemWeaponBean final : DatabaseBean {
	std::string item_id;
	sf::Time chop_cooldown;
	sf::FloatRect chop_rect;
	int chop_damage;
};