#pragma once

#include "global.h"
#include "Logger.h"
#include "Structs/QuestData.h"

#include "LuaBridge/LuaBridge.h"

// static helper class to load lua files for quests
class QuestLoader {
public:
	static QuestData loadQuest(const std::string& questID);

private:
	static const std::string QUEST_FOLDER;
};