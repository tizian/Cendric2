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
	Hint
};

// holds a part of the trigger content.
// the Trigger content type decides which of 
// the attributes are used.
struct TriggerContent {
	TriggerContent(TriggerContentType type_) : type(type_) {};
	TriggerContentType type;
	std::string firstStringAttribute;
	std::string secondStringAttribute;
	int integerAttribute;

	// executes the trigger on the given world screen
	static void executeTrigger(const TriggerContent& content, WorldScreen* screen);
};
