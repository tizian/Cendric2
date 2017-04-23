#pragma once

#include "Beans/DatabaseBean.h"

// a convertible item bean, directly from the database
struct ItemConvertibleBean final : DatabaseBean {
	std::string item_id;
	std::string convertible_item_id;
	int convertible_amount; // from 1 to max int
	int probability; // reaching from 0 to 100
};