#pragma once

#include <iostream>
#include "Beans/DatabaseBean.h"

// an item document quest bean, directly from the database
struct ItemDocumentQuestBean : DatabaseBean {
	std::string item_id;
	std::string quest_name;
	std::string quest_state;
	int quest_desc;
};