#include "FileIO/MerchantLoader.h"
#include "ResourceManager.h"

using namespace std;
using namespace luabridge;

MerchantData MerchantLoader::loadMerchant(const std::string& merchantID) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	MerchantData merchantData;

	std::string filename = std::string(g_resourceManager->getFilename(ResourceID::Npc_folder)) + merchantID + "/me_" + merchantID + ".lua";

	if (luaL_dofile(L, getPath(filename).c_str()) != 0) {
		g_logger->logError("MerchantLoader", "Cannot read lua script: " + getPath(filename));
		return merchantData;
	}

	lua_pcall(L, 0, 0, 0);

	LuaRef multiplier = getGlobal(L, "multiplier");
	if (multiplier.isNumber()) {
		float mult = multiplier.cast<float>();
		if (mult < 1.f) {
			g_logger->logWarning("MerchantLoader", "Merchant multiplier is smaller than 1, this is not allowed, the default (1.5f) is taken instead.");
		}
		else {
			merchantData.multiplier = mult;
		}
	}

	LuaRef fraction = getGlobal(L, "fraction");
	if (fraction.isString()) {
		merchantData.fraction = resolveFractionID(fraction.cast<std::string>());
	}

	LuaRef wares = getGlobal(L, "wares");
	if (wares.isTable()) {
		int i = 1; // in lua, the first element is 1, not 0. Like Eiffel haha.
		LuaRef element = wares[i];
		while (!element.isNil()) {
			LuaRef name = element[1];
			LuaRef amount = element[2];
			if (!name.isString() || !amount.isNumber()) {
				g_logger->logError("MerchantLoader", "File [" + filename + "]: wares table not resolved, no name or amount or of wrong type.");
				return merchantData;
			}
			merchantData.wares.insert({ name.cast<std::string>(), amount.cast<int>() });
			i++;
			element = wares[i];
		}
	}

	return merchantData;
}
