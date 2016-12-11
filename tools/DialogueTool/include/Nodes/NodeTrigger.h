#pragma once

#include "global.h"

#define RAW_TRIGGER_LENGTH 200

enum class TriggerType {
	Raw, // raw + direct trigger
	ChangeQuestState,
	AddQuestProgress,
	AddQuestDescription,
	AddConditionProgress,
	AddReputationProgress,
	AddHint,
	AddItem,
	EquipItem,
	RemoveItem,
	AddGold,
	RemoveGold,
	LearnSpell,
	StartLevel,
	StartMap,
	StartCutscene,
	SetGuild,
	gotoNode,
	MAX
};

// a node trigger that can be added and exported to lua
struct NodeTrigger {
	NodeTrigger(TriggerType type);

	TriggerType type;
	std::string s1;
	std::string s2;
	int i1 = 1;
	int i2 = 1;
	char rawTrigger[RAW_TRIGGER_LENGTH];

	std::string exportToLua() const;

	static const char* TRIGGER_TYPES;
};