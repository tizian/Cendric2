#pragma once

#include <iostream>
#include "Beans/DatabaseBean.h"

// a text bean, directly from the database
struct TextBean : DatabaseBean {
	std::string text_id;
	std::string text_type;
	std::string english;
	std::string german;
	std::string swiss_german;
};