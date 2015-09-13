#pragma once

#include <iostream>

#include "Enums/NPCState.h"
#include "Enums/QuestState.h"

enum class DialogueNodeType
{
	CendricTalking,
	NPCTalking,
	Descision
};

struct DialogueNode
{
	int tag;
	DialogueNodeType type;
	// it is an empty string if this is a descision
	std::string text;
	// this is an empty vector if it is not a descision
	std::vector<std::pair<std::string, int>> descisions;
	// The next tag. It may be 0 to infinity, but -1 means that the dialogue ends after that node
	int nextTag;
	std::map<std::string, QuestState> questStates;
	std::map<std::string, NPCState> npcStates;
	std::map<std::string, std::vector<std::string>> questProgress;
	// TODO: reputation progress
	// std::map<FractionID, int> reputationProgress;
};