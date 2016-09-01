#pragma once

#include <iostream>
#include "Beans/DatabaseBean.h"

// a spawn bean, directly from the database
struct SpawnBean : DatabaseBean {
	std::string spawn_id;
	std::string map_id;
	sf::Vector2f map_pos;
	std::string weapon_id;
	std::string armor_id;
};