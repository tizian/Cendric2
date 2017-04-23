#pragma once

#include <iostream>
#include "Beans/DatabaseBean.h"

// a levelitem frame bean, directly from the database
struct LevelitemFrameBean final : DatabaseBean {
	std::string item_id;
	int frame_nr;
	sf::IntRect texture_location;
};