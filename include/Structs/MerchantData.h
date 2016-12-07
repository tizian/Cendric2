#pragma once

#include "global.h"
#include "Enums/FractionID.h"

struct MerchantData {
	FractionID fraction = FractionID::VOID;
	// the multiplier for the selling prices (1.5 is the default)
	float multiplier = 1.5f;

	// the wares the npc offers for sale
	std::map<std::string, int> wares;
	// reputation needed to buy an item (default 0)
	std::map<std::string, int> reputation;
};