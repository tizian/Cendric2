#pragma once

#include "global.h"
#include "Logger.h"
#include "Structs/MerchantData.h"

#include "LuaBridge/LuaBridge.h"

// helper class to load lua files for trading
class MerchantLoader {
public:
	MerchantLoader();
	~MerchantLoader();
	MerchantData loadMerchant(const std::string& merchantID) const;
};