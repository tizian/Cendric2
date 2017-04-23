#pragma once

#include <iostream>
#include "Beans/DatabaseBean.h"

// a levelitem bean, directly from the database
struct LevelitemBean final : DatabaseBean {
	std::string item_id;
	sf::Vector2f sprite_offset;
	sf::Vector2f bounding_box;
	sf::Time frame_time;
};