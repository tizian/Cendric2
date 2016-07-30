#pragma once

#include "global.h"

class StartNode;

// A dialogue with conditions, choices and text
class Dialogue {
public:
	Dialogue(const std::string& npcID);
	~Dialogue();

	// returns whether the export was successful
	bool exportDialogue();

	// getters
	StartNode* getStartNode() const;

private:
	std::string m_npcID;
	StartNode* m_startNode;

private: 
	// exports all nodes to a lua string
	std::string exportToLua() const;
};