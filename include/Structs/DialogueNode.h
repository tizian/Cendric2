#pragma once

#include <iostream>

#include "Enums/QuestState.h"
#include "Enums/FractionID.h"

enum class DialogueNodeType {
	CendricTalking,
	NPCTalking,
	Choice,
	Trade
};

enum class DialogueNodeContentType {
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

// holds a part of the dialogue node content.
// the dialogue node content type decides which of 
// the attributes are used.
struct DialogeNodeContent {
	DialogeNodeContent(DialogueNodeContentType type_) : type(type_){};
	DialogueNodeContentType type;
	std::string firstStringAttribute;
	std::string secondStringAttribute;
	int integerAttribute;
};

struct DialogueNode {
	// the dialogue tag
	int tag;
	// The next tag. It may be 0 to infinity, but -1 means that the dialogue ends after that node
	int nextTag;
	// dialogue node type
	DialogueNodeType type;
	// it is an empty string if this is a choice
	std::string text;
	// this is an empty vector if it is not a choice
	std::vector<std::pair<std::string, int>> choices;
	// this is empty if it is not a trade
	std::string merchantID;
	// the content of this node, may be 0-n elements
	std::vector<DialogeNodeContent> content;
};