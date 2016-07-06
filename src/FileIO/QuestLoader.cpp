#include "FileIO/QuestLoader.h"

using namespace std;
using namespace luabridge;

const std::string QuestLoader::QUEST_FOLDER = "res/quest/";

QuestData QuestLoader::loadQuest(const std::string& questID) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	QuestData questData;
	questData.id = "";

	std::string filename = QUEST_FOLDER + questID + ".lua";

	if (luaL_dofile(L, getResourcePath(filename).c_str()) != 0) {
		g_logger->logError("QuestLoader", "Cannot read lua script: " + getResourcePath(filename));
		return questData;
	}

	lua_pcall(L, 0, 0, 0);

	LuaRef targets = getGlobal(L, "targets");
	if (targets.isTable()) {
		int i = 1; // in lua, the first element is 1, not 0. Like Eiffel haha.
		LuaRef element = targets[i];
		while (!element.isNil()) {
			LuaRef name = element[1];
			LuaRef amount = element[2];
			if (!name.isString() || !amount.isNumber()) {
				g_logger->logError("QuestLoader", "Quest [" + filename + "]: target table not resolved, no name or amount or of wrong type.");
				return questData;
			}
			questData.targets.insert({ name.cast<std::string>(), amount.cast<int>() });
			i++;
			element = targets[i];
		}
	}

	LuaRef collectibles = getGlobal(L, "collectibles");
	if (collectibles.isTable()) {
		int i = 1;
		LuaRef element = collectibles[i];
		while (!element.isNil()) {
			LuaRef id = element[1];
			LuaRef amount = element[2];
			if (!id.isString() || !amount.isNumber()) {
				g_logger->logError("QuestLoader", "Quest [" + filename + "]: collectibles table not resolved, no id or amount or of wrong type.");
				return questData;
			}
			questData.collectibles.insert({ id.cast<std::string>(), amount.cast<int>() });
			i++;
			element = collectibles[i];
		}
	}

	LuaRef conditions = getGlobal(L, "conditions");
	if (conditions.isTable()) {
		int i = 1;
		LuaRef element = conditions[i];
		while (element.isString()) {
			questData.conditions.insert(element.cast<std::string>());
			i++;
			element = conditions[i];
		}
	}

	questData.id = questID;
	return questData;
}
