#pragma once

#include <iostream>

#include "Enums/NPCState.h"
#include "Enums/QuestState.h"

enum class DialogueNodeType
{
	CendricTalking,
	NPCTalking,
	Choice
};

struct DialogueNode
{
	int tag;
	DialogueNodeType type;
	// it is an empty string if this is a choice
	std::string text;
	// this is an empty vector if it is not a choice
	std::vector<std::pair<std::string, int>> choices;
	// The next tag. It may be 0 to infinity, but -1 means that the dialogue ends after that node
	int nextTag;
	std::map<std::string, QuestState> questStates;
	std::map<std::string, NPCState> npcStates;
	std::map<std::string, std::vector<std::string>> questProgress;
	// TODO: reputation progress
	// std::map<FractionID, int> reputationProgress;
};