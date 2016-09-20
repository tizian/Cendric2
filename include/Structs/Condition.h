#pragma once

#include "global.h"

struct Condition {
	bool negative = false;
	std::string type;
	std::string name;
};