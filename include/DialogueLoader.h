#pragma once

#include "global.h"
#include "Dialogue.h"
#include "Enums/NPCState.h"
#include "Enums/QuestState.h"

#include "LuaBridge/LuaBridge.h"

class CharacterCore;

// helper class to load lua files for dialogues
class DialogueLoader
{
public:
	DialogueLoader(Dialogue& dialogue, CharacterCore* core);
	~DialogueLoader();
	void loadDialogue();

	// methods for questions about the current game state
	bool isNPCState(const std::string& npcID, const std::string& state) const;
	bool isQuestState(const std::string& questID, const std::string& state) const;

	// methods to create a node
	void createCendricNode(int tag, int nextTag, const std::string& text);
	void createNPCNode(int tag, int nextTag, const std::string& text);
	void createDescisionNode(int tag);
	
	// methods to add properties to that node
	void addDescision(int nextTag, const std::string& text);
	void changeNPCState(const std::string& npcID, const std::string& state);
	void changeQuestState(const std::string& questID, const std::string& state);
	void addQuestProgress(const std::string& questID, const std::string& progress);

	// finally, adding the node to the dialogue
	void addNode();
	void setRoot(int tag);

private:
	Dialogue& m_dialogue;
	CharacterCore* m_core;
	DialogueNode* m_currentNode = nullptr;
};