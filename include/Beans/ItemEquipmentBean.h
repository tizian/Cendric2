#pragma once

#include <iostream>
#include "Beans/DatabaseBean.h"

// an item equipment bean, directly from the database
struct ItemEquipmentBean : DatabaseBean {
	std::string item_id;
	std::string texture_path;
	int frames_walk;
	int frames_idle;
	int frames_jump;
	int frames_fight;
	int frames_climb1;
	int frames_climb2;
};