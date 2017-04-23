#pragma once

#include <iostream>
#include "Beans/DatabaseBean.h"

// an item attribute bean, directly from the database
struct ItemAttributeBean final : DatabaseBean {
	std::string item_id;
	int max_health;
	int health_regeneration;
	int haste;
	int critical;
	int heal;
	int dmg_physical;
	int dmg_fire;
	int dmg_ice;
	int dmg_shadow;
	int dmg_light;
	int res_physical;
	int res_fire;
	int res_ice;
	int res_shadow;
	int res_light;
};