#pragma once

#include "global.h"
#include "Dialogue.h"
#include "Enums/QuestState.h"

#include "LuaBridge/LuaBridge.h"

class CharacterCore;

// helper class to load lua files for dialogues
class DialogueLoader {
public:
	DialogueLoader(Dialogue& dialogue, CharacterCore* core);
	~DialogueLoader();
	void loadDialogue();

	// methods for questions about the current game state
	bool isQuestState(const std::string& questID, const std::string& state) const;
	bool isQuestComplete(const std::string& questID);
	bool isConditionFulfilled(const std::string& conditionType, const std::string& condition) const;

	// methods to create a node
	void createCendricNode(int tag, int nextTag, const std::string& text);
	void createNPCNode(int tag, int nextTag, const std::string& text);
	void createTradeNode(int tag, int nextTag, const std::string& text);
	void createChoiceNode(int tag);

	// methods to add properties to that node
	void addChoice(int nextTag, const std::string& text);
	void changeQuestState(const std::string& questID, const std::string& state);
	void addQuestProgress(const std::string& questID, const std::string& progress);
	void addConditionProgress(const std::string& conditionType, const std::string& condition);
	void addHint(const std::string& hint);
	void addItem(const std::string& itemID, int amount);
	void equipItem(const std::string& itemID);
	void removeItem(const std::string& itemID, int amount);
	void addGold(int amount);
	void removeGold(int amount);
	// TODO: learn spell, add reputation, etc.

	// finally, adding the node to the dialogue
	void addNode();
	void setRoot(int tag);

private:
	Dialogue& m_dialogue;
	CharacterCore* m_core;
	DialogueNode* m_currentNode = nullptr;
};