#pragma once

#include "global.h"

class Dialogue;
class DialogueNode;

// static class to load and save a dialogue file
class DialogueIO {
public:
	// returns whether the operation was successful
	static bool loadDialogue(const std::string& filePath, Dialogue** dialogue);
	// returns whether the operation was successful
	static bool saveDialogue(const std::string& filePath, Dialogue& dialogue);

private:
	static bool readPath(std::string& path, const std::string& line);
	// recursive method to write a node into the stack (writes all other nodes bound to it as well)
	static void writeNode(DialogueNode* node, std::string& stack, int indentation);

public:
	static const std::string NPC_ID;
	static const std::string TAG;
	static const std::string TYPE;
	static const std::string TRIGGER;
	static const std::string TRANSLATION_TAG;
	static const std::string TRANSLATION_EN;
	static const std::string TRANSLATION_DE;
	static const std::string TRANSLATION_CH;
	static const std::string CONDITION;
};