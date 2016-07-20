#pragma once

#include "global.h"
#include "Map/Dialogue.h"
#include "Enums/QuestState.h"

#include "LuaBridge/LuaBridge.h"
#include "Callbacks/WorldCallback.h"

class WorldScreen;

// helper class to load lua files for dialogues
class DialogueLoader {
public:
	DialogueLoader(Dialogue& dialogue, WorldScreen* screen);
	~DialogueLoader();
	void loadDialogue();

	// methods for questions about the current game state
	bool isQuestState(const std::string& questID, const std::string& state) const { return m_worldCallback->isQuestState(questID, state); }
	bool isQuestComplete(const std::string& questID) const { return m_worldCallback->isQuestComplete(questID); }
	bool isConditionFulfilled(const std::string& conditionType, const std::string& condition) const { return m_worldCallback->isConditionFulfilled(conditionType, condition); }
	bool isQuestConditionFulfilled(const std::string& quest, const std::string& condition) const { return m_worldCallback->isQuestConditionFulfilled(quest, condition); }
	bool hasItem(const std::string& item, int amount) const { return m_worldCallback->hasItem(item, amount); }
	int getReputation(const std::string& fractionID) const { return m_worldCallback->getReputation(fractionID); }

	// methods to create a node
	void createCendricNode(int tag, int nextTag, const std::string& text);
	void createNPCNode(int tag, int nextTag, const std::string& text);
	void createTradeNode(int tag, int nextTag);
	void createChoiceNode(int tag);

	// methods to add properties to that node (trigger properties)
	void changeQuestState(const std::string& questID, const std::string& state);
	void addQuestProgress(const std::string& questID, const std::string& progress);
	void addQuestDescription(const std::string& questID, int descriptionID);
	void addConditionProgress(const std::string& conditionType, const std::string& condition);
	void addReputationProgress(const std::string& fractionID, int amount);
	void addHint(const std::string& hint);
	void addItem(const std::string& itemID, int amount);
	void equipItem(const std::string& itemID);
	void removeItem(const std::string& itemID, int amount);
	void addGold(int amount);
	void removeGold(int amount);
	void learnSpell(int spellID);
	void startLevel(const std::string& levelID, int x, int y);
	void startMap(const std::string& mapID, int x, int y);
	void startCutscene(const std::string& cutsceneID);

	// special node properties
	void addChoice(int nextTag, const std::string& text);
	void gotoNode(int node);

	// finally, adding the node to the dialogue
	void addNode();
	void setRoot(int tag);

private:
	Dialogue& m_dialogue;
	WorldCallback* m_worldCallback;
	DialogueNode* m_currentNode = nullptr;
};