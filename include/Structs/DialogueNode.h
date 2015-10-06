#pragma once

#include <iostream>

#include "Enums/NPCState.h"
#include "Enums/QuestState.h"
#include "Enums/FractionID.h"

enum class DialogueNodeType {
	CendricTalking,
	NPCTalking,
	Choice,
	Trade
};

struct DialogueNode {
	int tag;
	DialogueNodeType type;
	// it is an empty string if this is a choice
	std::string text;
	// this is an empty vector if it is not a choice
	std::vector<std::pair<std::string, int>> choices;
	// this is empty if it is not a trade
	std::string merchantID;
	// The next tag. It may be 0 to infinity, but -1 means that the dialogue ends after that node
	int nextTag;
	std::map<std::string, QuestState> questStates;
	std::map<std::string, NPCState> npcStates;
	std::map<std::string, std::string> questProgress;
	std::map<FractionID, int> reputationProgress;

	// removes (amount < 0) or adds (amount > 0) items to cendrics inventory. The same goes for gold.
	std::map<std::string, int> itemChanges;
	int goldChanges = 0;
};