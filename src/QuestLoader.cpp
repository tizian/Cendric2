#include "QuestLoader.h"

using namespace std;
using namespace luabridge;

QuestLoader::QuestLoader()
{
}

QuestLoader::~QuestLoader()
{
}

QuestData QuestLoader::loadQuest(const std::string& questID) const
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	QuestData questData;
	questData.id = "";

	std::string filename = QUEST_FOLDER + questID + ".lua";

	if (luaL_dofile(L, filename.c_str()) != 0)
	{
		g_logger->logError("QuestLoader", "Cannot read lua script: " + filename);
		return questData;
	}

	lua_pcall(L, 0, 0, 0);

	LuaRef title = getGlobal(L, "title");
	LuaRef description = getGlobal(L, "description");
	if (!title.isString() || !description.isString())
	{
		g_logger->logError("QuestLoader", "Quest [" + filename + "] has no title or description tag or of wrong type.");
		return questData;
	}

	questData.title = title.cast<std::string>();
	questData.description = description.cast<std::string>();

	LuaRef targets = getGlobal(L, "targets");
	if (!targets.isNil() && targets.isTable())
	{
		int i = 1; // in lua, the first element is 1, not 0. Like Eiffel haha.
		LuaRef element = targets[i];
		while (!element.isNil())
		{
			LuaRef name = element[1];
			LuaRef amount = element[2];
			if (!name.isString() || !amount.isNumber())
			{
				g_logger->logError("QuestLoader", "Quest [" + filename + "]: target table not resolved, no name or amount or of wrong type.");
				return questData;
			}
			questData.targets.insert({name.cast<std::string>(), amount.cast<int>()});
			i++;
			element = targets[i];
		}
	}

	LuaRef collectibles = getGlobal(L, "collectibles");
	if (!collectibles.isNil() && collectibles.isTable())
	{
		int i = 1;
		LuaRef element = collectibles[i];
		while (!element.isNil())
		{
			LuaRef id = element[1];
			LuaRef amount = element[2];
			if (!id.isString() || !amount.isNumber())
			{
				g_logger->logError("QuestLoader", "Quest [" + filename + "]: collectibles table not resolved, no id or amount or of wrong type.");
				return questData;
			}
			questData.collectibles.insert({ id.cast<std::string>(), amount.cast<int>() });
			i++;
			element = collectibles[i];
		}
	}

	LuaRef conditions = getGlobal(L, "conditions");
	if (!conditions.isNil() && conditions.isTable())
	{
		int i = 1;
		LuaRef element = conditions[i];
		while (element.isString())
		{
			questData.conditions.insert(element.cast<std::string>());
			i++;
			element = targets[i];
		}
	}

	questData.id = questID;
	return questData;
}
