#pragma once

#include <iostream>

#include "Enums/QuestState.h"
#include "Enums/FractionID.h"
#include "Structs/TriggerContent.h"

enum class DialogueNodeType {
	CendricTalking,
	NPCTalking,
	Choice,
	Trade
};

struct DialogueNode {
	// the dialogue tag
	int tag;
	// The next tag. It may be 0 to infinity, but 
	// -1 means that the dialogue ends after that node and
	// -2 tells the dialogue to reload and start at root after that node (is useful if conditions change)
	int nextTag;
	// if this is set (> -1), the dialogue will reload after that and jump to the node with this tag
	int reloadTag = -1;
	// dialogue node type
	DialogueNodeType type;
	// it is an empty string if this is a choice
	std::string text;
	// this is an empty vector if it is not a choice
	std::vector<std::pair<std::string, int>> choices;
	// this is empty if it is not a trade
	std::string merchantID;
	// the content of this node, may be 0-n elements
	std::vector<TriggerContent> content;
};