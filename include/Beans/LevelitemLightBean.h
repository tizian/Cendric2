#pragma once

#include <iostream>
#include "Beans/DatabaseBean.h"

// a levelitem light bean, directly from the database
struct LevelitemLightBean : DatabaseBean {
	std::string item_id;
	sf::Vector2f light_offset;
	sf::Vector2f light_radius;
	float brightness;
};