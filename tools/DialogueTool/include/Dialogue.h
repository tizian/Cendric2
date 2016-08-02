#pragma once

#include "global.h"
#include <set>

class StartNode;
class DialogueNode;

// A dialogue with conditions, choices and text
class Dialogue {
public:
	Dialogue(const std::string& npcID);
	~Dialogue();

	// generates a new, unused tag
	int generateTag();
	// frees a tag that is no longer used
	void freeTag(int tag);

	// returns whether the export was successful
	bool exportDialogue();

	// getters
	StartNode* getStartNode() const;
	const std::string& getNpcID() const;

private:
	std::string m_npcID;
	StartNode* m_startNode;
	std::set<int> m_usedTags;

private: 
	// exports all nodes to a lua string
	std::string exportToLua() const;
	void recursiveLuaExport(DialogueNode* node, std::string& stack,  int indentationLevel) const;

	// exports all nodes to a sql string
	std::string exportToSQL() const;
	void recursiveSQLExport(DialogueNode* node, std::string& stack) const;
};