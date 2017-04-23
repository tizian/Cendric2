#pragma once

#include "global.h"
#include "Logger.h"
#include "Structs/MerchantData.h"

#include "LuaBridge/LuaBridge.h"

class CharacterCore;

// static helper class to load lua files for trading
class MerchantLoader final {
private:
	MerchantLoader() {}
public:
	static MerchantData loadMerchant(const std::string& merchantID, const CharacterCore* core);
};