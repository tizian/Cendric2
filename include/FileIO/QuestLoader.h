#pragma once

#include "global.h"
#include "Structs/QuestData.h"

// static helper class to load lua files for quests
class QuestLoader final {
	QuestLoader() = default;
public:
	static QuestData loadQuest(const std::string& questID);

private:
	static const std::string QUEST_FOLDER;
};