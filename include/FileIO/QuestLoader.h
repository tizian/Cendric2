#pragma once

#include "global.h"
#include "Logger.h"
#include "Structs/QuestData.h"

#include "LuaBridge/LuaBridge.h"

// helper class to load lua files for quests
class QuestLoader {
public:
	QuestLoader();
	~QuestLoader();
	QuestData loadQuest(const std::string& questID) const;

private:
	const std::string QUEST_FOLDER = "res/quest/";
};