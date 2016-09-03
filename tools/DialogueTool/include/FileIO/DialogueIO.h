#pragma once

#include "global.h"

class Dialogue;
class DialogueNode;
class TriggerableNode;
struct LinkNode;

// static class to load and save a dialogue file
class DialogueIO {
public:
	// returns whether the operation was successful
	static bool loadDialogue(const std::string& filePath, Dialogue** dialogue);
	// returns whether the operation was successful
	static bool saveDialogue(const std::string& filePath, Dialogue& dialogue);

private:
	// recursive method to write a node into the stack (writes all other nodes bound to it as well)
	static void writeNode(DialogueNode* node, std::string& stack, int indentation);

	static bool readNpcID(std::string& id, const std::string& line);
	static bool createNewNode(DialogueNode** node, const std::string& line);
	static bool createNewLink(LinkNode** node, const std::string& line);
	static bool addLinkCondition(LinkNode* node, const std::string& line);
	static bool addLinkTag(LinkNode* node, const std::string& line);
	static bool addNodeTag(DialogueNode* node, const std::string& line);
	static bool addLinkTranslation(LinkNode* node, const std::string& line);
	static bool addNodeTranslation(TriggerableNode* node, const std::string& line);
	static bool addNodeTrigger(TriggerableNode* node, const std::string& line);

public:
	static const std::string NPC_ID;
	static const std::string TAG;
	static const std::string TRIGGER;
	static const std::string TRANSLATION;
	static const std::string TRANSLATION_TAG;
	static const std::string TRANSLATION_EN;
	static const std::string TRANSLATION_DE;
	static const std::string TRANSLATION_CH;
	static const std::string TRANSLATION_ITEM_ID;
	static const std::string TRANSLATION_ITEM_AMOUNT;
	static const std::string CONDITION;
};