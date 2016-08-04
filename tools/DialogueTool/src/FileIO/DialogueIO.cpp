#include "FileIO/DialogueIO.h"
#include "Dialogue.h"
#include "ApplicationState.h"
#include "Nodes/StartNode.h"

#include <fstream>
#include <sstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

const std::string DialogueIO::NPC_ID = "npc.id";
const std::string DialogueIO::TAG = "tag";
const std::string DialogueIO::TYPE = "type";
const std::string DialogueIO::TRIGGER = "trigger";
const std::string DialogueIO::TRANSLATION_TAG = "translation.tag";
const std::string DialogueIO::TRANSLATION_EN = "translation.en";
const std::string DialogueIO::TRANSLATION_DE = "translation.de";
const std::string DialogueIO::TRANSLATION_CH = "translation.ch";
const std::string DialogueIO::CONDITION = "condition";

static inline void ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
}

bool DialogueIO::loadDialogue(const std::string& filePath, Dialogue** dialogue) {
	delete *dialogue;
	*dialogue = nullptr;

	StartNode* startNode = new StartNode();
	std::string npcID;
	
	std::string line;
	std::ifstream dialoguePath(filePath);
	bool noError = true;
	if (dialoguePath.is_open()) {
		while (getline(dialoguePath, line)) {
			ltrim(line);
			if (line.empty() || line.compare(0, 1, "#") == 0) {
				continue;
			}
			/*else if (line.compare(0, SQL_PATH.size(), SQL_PATH) == 0) {
				noError = readPath(data.sqlFolder, line);
			}
			else if (line.compare(0, NPC_PATH.size(), NPC_PATH) == 0) {
				noError = readPath(data.npcFolder, line);
			}
			else if (line.compare(0, DIALOGUE_PATH.size(), DIALOGUE_PATH) == 0) {
				noError = readPath(data.dialogueFolder, line);
			}
			else {
				ERROR("[Dialogue IO]: Unknown tag found in dialogue file in line: " + line);
				noError = false;
			}*/
			if (!noError) {
				break;
			}
		}
		dialoguePath.close();

		if (!noError) {
			delete startNode;
			return false;
		}
	}
	else {
		ERROR("[Dialogue IO]: Unable to open dialogue file. Dialogue could not be loaded.");
		g_state->setConfiguration(DEFAULT_CONFIGURATION);
		delete startNode;
		return false;
	}

	// get file name from dialogue path
	std::string dialogueName = getFileName(filePath);
	
	*dialogue = new Dialogue(npcID, dialogueName, startNode);
	return true;
}


bool DialogueIO::readPath(std::string& path, const std::string& line) {
	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		ERROR("[Configuration]: No colon found after display mode tag or no value after colon.");
		return false;
	}
	// enforce 100 limit to avoid buffer overflow
	path = line.substr(colon + 1).substr(0, 99);
	return true;
}

bool DialogueIO::saveDialogue(const std::string& filePath, Dialogue& dialogue) {
	std::ofstream dialogueFile(filePath, std::ios::trunc);
	if (dialogueFile.is_open()) {
		// write header
		dialogueFile << "# dia file for dialogue for npc: ";
		dialogueFile << dialogue.getNpcID(); 
		dialogueFile << "\n\n";
		dialogueFile << DialogueIO::NPC_ID + ":" + dialogue.getNpcID();
		dialogueFile << "\n\n";
		// write node stack
		std::string stack = "";
		writeNode(dialogue.getStartNode(), stack, 0);
		dialogueFile << stack;
		dialogueFile.close();
	}
	else {
		ERROR("[Dialogue IO]: Unable to open file: " + filePath);
		return false;
	}
	return true;
}

void DialogueIO::writeNode(DialogueNode* node, std::string& stack, int indentation) {
	if (node == nullptr) return;
	stack.append(tabs(indentation) + "{\n");
	stack.append(node->exportToDia(indentation + 1));

	for (auto& child : node->getLinkNodes()) {
		stack.append(tabs(indentation + 1) + "[\n");
		stack.append(child->exportToDia(indentation + 2));
		stack.append(tabs(indentation + 1) + "]\n");
	}

	stack.append(tabs(indentation) + "}\n");

	for (auto& child : node->getLinkNodes()) {
		writeNode(child->nextNode, stack, indentation);
	}
}


