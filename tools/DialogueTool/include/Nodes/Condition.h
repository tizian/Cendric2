#pragma once

#include "global.h"

enum class ConditionType {
	VOID,
	Condition,
	QuestState,
	QuestComplete,
	HasItem,
	GetReputation,
	GetGuild,
	IsSpellLearned,
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