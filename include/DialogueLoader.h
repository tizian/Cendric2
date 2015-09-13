#pragma once

#include "global.h"
#include "Dialogue.h"
#include "Enums/NPCState.h"
#include "Enums/QuestState.h"

#include "LuaBridge/LuaBridge.h"

class CharacterCore;

// helper class to load lua files for dialogues
// current class to mess around with lua
class DialogueLoader
{
public:
	DialogueLoader(Dialogue& dialogue, CharacterCore* core);
	~DialogueLoader();
	void loadDialogue();

	// methods for lua
	bool isNPCNeverTalkedTo(const std::string& npcID) const;
	bool isQuestCompleted(const std::string& questID) const;

	// methods to create a node
	void setCendricTalking();
	void setNPCTalking();
	void setText(const std::string& text);
	void setNextTag(int tag);

	// finally, adding the node to the dialogue
	void addNode(int tag);
	void setRoot(int tag);

private:
	Dialogue& m_dialogue;
	CharacterCore* m_core;
	DialogueNode* m_currentNode = nullptr;
};