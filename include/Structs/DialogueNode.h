#pragma once

#include <iostream>

#include "Enums/NPCID.h"
#include "Enums/NPCState.h"
#include "Enums/QuestID.h"
#include "Enums/QuestState.h"

struct DialogueNode
{
	// it is an empty string if this is a descision
	std::string text;
	bool isNPCSpeaking;
	// this is an empty vector if it is not a descision
	std::vector<std::pair<std::string, int>> descisions;
	// The next tag. It may be 0 to infinity, but -1 means that the dialogue ends after that node
	int nextTag;
	std::map<QuestID, QuestState> questProgress;
	std::map<NPCID, NPCState> npcProgress;
	// TODO: reputation progress
	// std::map<FractionID, int> reputationProgress;
};