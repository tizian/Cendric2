#pragma once

#include "global.h"
#include <set>

class StartNode;
class DialogueNode;

// A dialogue with conditions, choices and text
class Dialogue final {
public:
	Dialogue(const std::string& npcID);
	Dialogue(const std::string& npcID, const std::string& dialogueName, StartNode* startNode);
	~Dialogue();

	// generates a new, unused tag
	int generateTag();
	// frees a tag that is no longer used
	void freeTag(int tag);

	// returns whether the export was successful
	bool exportDialogue();
	// exports to a dia file and returns whether the export was successful
	bool exportToDia();
	// used for the dialogue file name
	void setDialogueName(const std::string& dialogueName);

	// getters
	StartNode* getStartNode();
	const std::string& getNpcID() const;
	const std::string& getDialogueName() const;

private:
	std::string m_npcID;
	std::string m_dialogueName;
	StartNode* m_startNode;
	std::set<int> m_usedTags;

private: 
	// exports all nodes to a lua string
	std::string exportToLua() const;
	void recursiveLuaExport(DialogueNode* node, std::string& stack,  int indentationLevel) const;

	// exports all nodes to a sql string
	std::string exportToSQL() const;
	void recursiveSQLExport(DialogueNode* node, std::string& stack) const;

	void recursiveCalculateUsedTags(DialogueNode* node);
};