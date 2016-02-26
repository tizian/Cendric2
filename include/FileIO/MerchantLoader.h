#pragma once

#include "global.h"
#include "Logger.h"
#include "Structs/MerchantData.h"

#include "LuaBridge/LuaBridge.h"

// static helper class to load lua files for trading
class MerchantLoader {
public:
	static MerchantData loadMerchant(const std::string& merchantID);
};