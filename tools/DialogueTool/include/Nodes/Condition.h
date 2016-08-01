#pragma once

#include "global.h"

enum class QuestState {
	VOID,
	Started, // a quest is started when it is accepted
	Completed, // a quest is completed when the reward is received
	Failed, // a quest has failed when it cannot be completed anymore (quest-npc is killed or something)
	MAX
};

inline QuestState resolveQuestState(const std::string& state) {
	if (state.compare("void") == 0) return QuestState::VOID;
	if (state.compare("started") == 0) return QuestState::Started;
	if (state.compare("completed") == 0) return QuestState::Completed;
	if (state.compare("failed") == 0) return QuestState::Failed;
	return QuestState::MAX;
}

enum class ConditionType {
	VOID,
	Condition,
	QuestState,
	QuestComplete,
	HasItem,
	MAX
};

// a condition that can be evaluated in lua.
// its type determines which members need to be evaluated.
struct Condition {
	ConditionType type;
	std::string s1;
	std::string s2;
	int i1 = 1;

	std::string exportToLua() const;

	static const char* CONDITION_TYPES;
};