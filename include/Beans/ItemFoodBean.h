#pragma once

#include <iostream>
#include "Beans/DatabaseBean.h"

// an item food bean, directly from the database
struct ItemFoodBean : DatabaseBean {
	std::string item_id;
	sf::Time food_duration;
	bool is_cookable;
	std::string cooked_item_id;
};