#include "FileIO/QuestLoader.h"
#include "LuaBridge/LuaBridge.h"
#include "Logger.h"

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

	LuaRef mainQuest = getGlobal(L, "main_quest");
	if (mainQuest.isBoolean()) {
		questData.isMainQuest = mainQuest.cast<bool>();
	}

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

	LuaRef markers = getGlobal(L, "markers");
	if (markers.isTable()) {
		int i = 1;
		LuaRef marker = markers[i];
		while (!marker.isNil()) {
			LuaRef map = marker["map"];
			LuaRef position = marker["position"];
			LuaRef step = marker["step"];
			if (!map.isString() || !step.isNumber() || !position.isTable()) {
				g_logger->logError("QuestLoader", "Quest [" + filename + "]: marker could not be resolved, map, position or step missing or wrong type.");
				return questData;
			}

			LuaRef posX = position[1];
			LuaRef posY = position[2];

			if (!posX.isNumber() || !posY.isNumber()) {
				g_logger->logError("QuestLoader", "Quest [" + filename + "]: marker could not be resolved, position has wrong format.");
				return questData;
			}

			QuestMarkerData data;
			data.mapId = map.cast<std::string>();
			data.step = step.cast<int>();
			data.position.x = posX.cast<float>();
			data.position.y = posY.cast<float>();

			questData.questMarkers.push_back(data);
			marker = markers[++i];
		}
	}

	questData.id = questID;
	return questData;
}
