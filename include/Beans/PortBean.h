#pragma once

#include <iostream>
#include "Beans/DatabaseBean.h"

// a port bean, directly from the database
struct PortBean : DatabaseBean {
	std::string port_id;
	std::string map_id;
	sf::Vector2f map_pos;
	std::string weapon_id;
	std::string armor_id;
};