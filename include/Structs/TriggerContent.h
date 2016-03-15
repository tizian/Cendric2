#pragma once

#include "global.h"

class WorldScreen;

// those can be triggered by triggers, dialogues, routines, npc behavior, etc.
enum class TriggerContentType {
	QuestStateChange,
	QuestConditionProgress,
	QuestDescriptionProgress,
	ConditionProgress,
	ReputationProgress,
	ItemChange,
	GoldChange,
	ItemEquip,
	Hint,
	MapEntry,
	LevelEntry,
	Cutscene,
};

// holds a part of the trigger content.
// the trigger content type decides which of 
// the attributes are used.
struct TriggerContent {
	TriggerContent(TriggerContentType type_) : type(type_) {};
	TriggerContentType type;
	std::string s1 = "";
	std::string s2 = "";
	int i1 = 0;
	int i2 = 0;

	// executes the trigger on the given world screen
	static void executeTrigger(const TriggerContent& content, WorldScreen* screen);
};
