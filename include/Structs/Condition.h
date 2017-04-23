#pragma once

#include "global.h"

struct Condition final {
	bool negative = false;
	std::string type;
	std::string name;
};