#pragma once

#include "global.h"
#include "ResourceManager.h"
#include "TextProvider.h"
#include "Structs/DialogueNode.h"

#include "Enums/DialogueID.h"

class CharacterCore;
class DialogueWindow;

// A dialogue with conditions, choices and text
class Dialogue
{
public:
	void load(DialogueID id, CharacterCore* core, DialogueWindow* window);
	DialogueID getID() const;
	void addNode(int tag, const DialogueNode& node);
	void setNextNode(int tag);
	void setRoot(int root);
	bool updateWindow();
	
private:
	CharacterCore* m_core;
	DialogueWindow* m_window;
	DialogueNode* m_currentNode;
	DialogueID m_id;
	std::map<int, DialogueNode> m_nodes;
};