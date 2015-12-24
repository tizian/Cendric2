#pragma once

#include <iostream>
#include "Beans/DatabaseBean.h"

// an item equipment bean, directly from the database
struct ItemEquipmentBean : DatabaseBean {
	std::string item_id;
	std::string texture_path;
};