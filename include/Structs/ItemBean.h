#pragma once

#include <iostream>

#include "Enums/ItemID.h"
#include "Enums/ItemType.h"

struct ItemBean
{
	std::string description;
	ItemID id;
	ItemType type;
	ResourceID icon;
	int value;
};