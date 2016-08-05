#include "FileIO/DialogueIO.h"
#include "Dialogue.h"
#include "ApplicationState.h"
#include "Nodes/StartNode.h"
#include "Nodes/NPCNode.h"
#include "Nodes/CendricNode.h"
#include "Nodes/TradeNode.h"
#include "Nodes/ChoiceNode.h"
#include "Nodes/NodeTranslation.h"
#include "Nodes/NodeTrigger.h"

#include <fstream>
#include <sstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <map>

const std::string DialogueIO::NPC_ID = "npc.id";
const std::string DialogueIO::TAG = "tag";
const std::string DialogueIO::TRIGGER = "trigger";
const std::string DialogueIO::TRANSLATION = "translation";
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

	std::string npcID;

	DialogueNode* currentNode = nullptr;
	LinkNode* currentLink = nullptr;
	std::vector<DialogueNode*> nodes;
	
	std::string line;
	std::ifstream dialoguePath(filePath);
	bool noError = true;
	if (dialoguePath.is_open()) {
		while (getline(dialoguePath, line)) {
			ltrim(line);
			if (line.empty() || line.compare(0, 1, "#") == 0) {
				continue;
			}
			else if (line.compare(0, NPC_ID.size(), NPC_ID) == 0) {
				noError = readNpcID(npcID, line);
			}
			else if (line.compare(0, 1, "{") == 0) {
				noError = createNewNode(&currentNode, line);
				if (noError && currentNode != nullptr) {
					nodes.push_back(currentNode);
				}
			}
			else if (line.compare(0, 1, "}") == 0) {
				if (currentNode == nullptr) {
					ERROR("[DialogueIO]: Cannot close a node that has not yet been created. (start with {)");
					noError = false;
				}
				else {
					currentNode = nullptr;
				}
			}
			else if (line.compare(0, 1, "[") == 0) {
				if (currentNode == nullptr) {
					ERROR("[DialogueIO]: Cannot add a child for a node that has not yet been created. (start with {)");
					noError = false;
				}
				else {
					noError = createNewLink(&currentLink, line);
					if (noError && currentLink != nullptr) {
						currentNode->addLinkNode(currentLink);
					}
				}
			}
			else if (line.compare(0, 1, "]") == 0) {
				if (currentLink == nullptr) {
					ERROR("[DialogueIO]: Cannot close a link node that has not yet been created. (start with [)");
					noError = false;
				}
				else {
					currentLink = nullptr;
				}
			}
			else if (line.compare(0, TAG.size(), TAG) == 0) {
				if (currentLink != nullptr) {
					noError = addLinkTag(currentLink, line);
				}
				else if (currentNode != nullptr) {
					noError = addNodeTag(currentNode, line);
				}
				else {
					ERROR("[DialogueIO]: Cannot add a tag to a node that has not yet been created. (start with {)");
					noError = false;
				}
			}
			else if (line.compare(0, CONDITION.size(), CONDITION) == 0) {
				if (currentLink != nullptr) {
					noError = addLinkCondition(currentLink, line);
				}
				else {
					ERROR("[DialogueIO]: Cannot add a condition to a link node that has not yet been created. (start with [)");
					noError = false;
				}
			}
			else if (line.compare(0, TRANSLATION.size(), TRANSLATION) == 0) {
				if (currentLink != nullptr) {
					noError = addLinkTranslation(currentLink, line);
				}
				else if (currentNode != nullptr) {
					if (TriggerableNode* tNode = dynamic_cast<TriggerableNode*>(currentNode)) {
						noError = addNodeTranslation(tNode, line);
					}
					else {
						ERROR("[DialogueIO]: Can only add translations to NPC and Cendric nodes.");
						noError = false;
					}
				}
				else {
					ERROR("[DialogueIO]: Cannot add a translation to a node that has not yet been created. (start with {)");
					noError = false;
				}
			}
			else if (line.compare(0, TRIGGER.size(), TRIGGER) == 0) {
				if (currentLink != nullptr) {
					ERROR("[DialogueIO]: Cannot add triggers to link nodes");
					noError = false;
				}
				else if (currentNode != nullptr) {
					if (TriggerableNode* tNode = dynamic_cast<TriggerableNode*>(currentNode)) {
						noError = addNodeTrigger(tNode, line);
					}
					else {
						ERROR("[DialogueIO]: Can only add triggers to NPC and Cendric nodes");
						noError = false;
					}
				}
				else {
					ERROR("[DialogueIO]: Cannot add a trigger to a node that has not yet been created. (start with {)");
					noError = false;
				}
			}
			else {
				ERROR("[Dialogue IO]: Unknown tag found in dialogue file in line: " + line);
				noError = false;
			}
			if (!noError) {
				break;
			}
		}
		dialoguePath.close();

		if (!noError) {
			for (auto it : nodes) {
				delete it;
			}
			return false;
		}
	}
	else {
		ERROR("[Dialogue IO]: Unable to open dialogue file. Dialogue could not be loaded.");
		for (auto it : nodes) {
			delete it;
		}
		return false;
	}

	// get file name from dialogue path
	std::string dialogueName = getFileName(filePath);

	StartNode* startNode = nullptr;
	std::map<int, DialogueNode*> tagToNode;

	// tag them
	for (auto node : nodes) {
		if (tagToNode.find(node->m_tag) != tagToNode.end()) {
			ERROR("[Dialogue IO] duplicate node tag found: " + std::to_string(node->m_tag));
			for (auto it : nodes) {
				delete it;
			}
			return false;
		}
		tagToNode.insert({node->m_tag, node});
		if (StartNode* startNode_ = dynamic_cast<StartNode*>(node)) {
			if (startNode != nullptr) {
				ERROR("[DialogueIO]: More than one start node found in dia file!");
				for (auto it : nodes) {
					delete it;
				}
				return false;
			}
			startNode = startNode_;
		}
	}

	// check for start node
	if (startNode == nullptr) {
		ERROR("[DialogueIO]: No start node found in dia file!");
		for (auto it : nodes) {
			delete it;
		}
		return false;
	}

	// link them
	for (auto node : nodes) {
		for (auto link : node->getLinkNodes()) {
			if (link->ioNextTag == -1) {
				delete link->nextNode;
				link->nextNode = nullptr;
				link->isReload = false;
			}
			else if (link->ioNextTag == -2) {
				delete link->nextNode;
				link->nextNode = nullptr;
				link->isReload = true;
			}
			else {
				if (tagToNode.find(link->ioNextTag) == tagToNode.end()) {
					ERROR("[Dialogue IO] link node could not be linked, reference not found. tag: " + std::to_string(link->ioNextTag));
					for (auto it : nodes) {
						delete it;
					}
					return false;
				}
				delete link->nextNode;
				link->nextNode = tagToNode.at(link->ioNextTag);
			}
		}
	}
	
	*dialogue = new Dialogue(npcID, dialogueName, startNode);
	return true;
}

bool DialogueIO::readNpcID(std::string& id, const std::string& line) {
	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		ERROR("[DialogueIO]: No colon found after npc node tag or no value after colon.");
		return false;
	}
	id = line.substr(colon + 1);
	return true;
}

bool DialogueIO::createNewNode(DialogueNode** node, const std::string& line) {
	if (*node != nullptr) {
		ERROR("[DialogueIO]: Cannot start new node after old one is still open (close with })");
		return false;
	}
	int tag = std::atoi(line.substr(1).c_str());
	DialogueNodeType type = static_cast<DialogueNodeType>(tag);
	if (type <= DialogueNodeType::End || type > DialogueNodeType::Start) {
		ERROR("[DialogueIO]: Dialogue node type not recognized: " + std::to_string(tag));
		return false;
	}
	switch (type) {
		break;
	case DialogueNodeType::NPC:
		*node = new NpcNode(0);
		break;
	case DialogueNodeType::Choice:
		*node = new ChoiceNode(0);
		break;
	case DialogueNodeType::Cendric:
		*node = new CendricNode(0);
		break;
	case DialogueNodeType::Trade:
		*node = new TradeNode(0);
		break;
	case DialogueNodeType::Start:
		*node = new StartNode(0);
		break;
	default:
		return false;
	}

	return true;
}

bool DialogueIO::addLinkTag(LinkNode* node, const std::string& line) {
	if (node == nullptr) return false;
	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		ERROR("[DialogueIO]: No colon found after link 'tag' tag or no value after colon.");
		return false;
	}
	int tag = std::atoi(line.substr(colon + 1).c_str());
	if (tag <= 0 && (tag != -1 && tag != -2)) {
		ERROR("[DialogueIO]: Cannot add link tag: " + std::to_string(tag));
		return false;
	}
	node->ioNextTag = tag;
	return true;
}

bool DialogueIO::addLinkCondition(LinkNode* node, const std::string& line) {
	if (node == nullptr) return false;
	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		ERROR("[DialogueIO]: No colon found after link 'condition' tag or no value after colon.");
		return false;
	}

	std::string conditionString = line.substr(colon + 1);
	if (conditionString.size() < 2000) {
		delete node->condition;
		NodeCondition* condition = new NodeCondition(NodeConditionType::Raw);
		strcpy(condition->getConditionString(), conditionString.c_str());
		node->condition = condition; 
	}
	else {
		ERROR("[DialogueIO]: Condition too long to fit (2000 chars max)");
		return false;
	}

	return true;
}

bool DialogueIO::addNodeTag(DialogueNode* node, const std::string& line) {
	if (node == nullptr) return false;
	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		ERROR("[DialogueIO]: No colon found after 'tag' tag or no value after colon.");
		return false;
	}
	int tag = std::atoi(line.substr(colon + 1).c_str());
	if (tag <= 0 && !(node->getType() == DialogueNodeType::Start && tag == -1)) {
		ERROR("[DialogueIO]: Node tag must be > 0. Cannot add tag: " + std::to_string(tag));
		return false;
	}
	node->m_tag = tag;
	return true;
}

bool DialogueIO::addLinkTranslation(LinkNode* node, const std::string& line) {
	if (node == nullptr) return false;
	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		ERROR("[DialogueIO]: No colon found after translation tag or no value after colon.");
		return false;
	}
	std::string translation = line.substr(colon + 1);
	if (node->translation == nullptr) {
		node->translation = new NodeTranslation(0, "");
	}
	if (line.compare(0, TRANSLATION_TAG.size(), TRANSLATION_TAG) == 0 && translation.size() < 50) {
		strcpy(node->translation->tag, translation.c_str());
	}
	else if (line.compare(0, TRANSLATION_EN.size(), TRANSLATION_EN) == 0 && translation.size() < MAX_DIALOGUE_SIZE) {
		strcpy(node->translation->englishTranslation, translation.c_str());
	}
	else if (line.compare(0, TRANSLATION_DE.size(), TRANSLATION_DE) == 0 && translation.size() < MAX_DIALOGUE_SIZE) {
		strcpy(node->translation->germanTranslation, translation.c_str());
	}
	else if (line.compare(0, TRANSLATION_CH.size(), TRANSLATION_CH) == 0 && translation.size() < MAX_DIALOGUE_SIZE) {
		strcpy(node->translation->swissgermanTranslation, translation.c_str());
	}
	else {
		ERROR("[DialogueIO]: translation could not be read, wrong tag or too long to fit.");
		return false;
	}
	return true;
}

bool DialogueIO::addNodeTranslation(TriggerableNode* node, const std::string& line) {
	if (node == nullptr) return false;

	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		ERROR("[DialogueIO]: No colon found after translation tag or no value after colon.");
		return false;
	}
	std::string translation = line.substr(colon + 1);
	if (node->m_translation == nullptr) {
		node->m_translation = new NodeTranslation(0, "");
	}
	if (line.compare(0, TRANSLATION_TAG.size(), TRANSLATION_TAG) == 0 && translation.size() < 50) {
		strcpy(node->m_translation->tag, translation.c_str());
	}
	else if (line.compare(0, TRANSLATION_EN.size(), TRANSLATION_EN) == 0 && translation.size() < MAX_DIALOGUE_SIZE) {
		strcpy(node->m_translation->englishTranslation, translation.c_str());
	}
	else if (line.compare(0, TRANSLATION_DE.size(), TRANSLATION_DE) == 0 && translation.size() < MAX_DIALOGUE_SIZE) {
		strcpy(node->m_translation->germanTranslation, translation.c_str());
	}
	else if (line.compare(0, TRANSLATION_CH.size(), TRANSLATION_CH) == 0 && translation.size() < MAX_DIALOGUE_SIZE) {
		strcpy(node->m_translation->swissgermanTranslation, translation.c_str());
	}
	else {
		ERROR("[DialogueIO]: translation could not be read, wrong tag or too long to fit.");
		return false;
	}
	return true;
}

bool DialogueIO::addNodeTrigger(TriggerableNode* node, const std::string& line) {
	if (node == nullptr) return false;

	size_t colon = line.find(':');
	if (colon == std::string::npos || line.length() < colon + 1) {
		ERROR("[DialogueIO]: No colon found after trigger tag or no value after colon.");
		return false;
	}
	std::string triggerString = line.substr(colon + 1);
	if (triggerString.size() < 200) {
		NodeTrigger* trigger = new NodeTrigger(TriggerType::Raw);
		strcpy(trigger->rawTrigger, triggerString.c_str());
		node->m_triggers.push_back(trigger);
	}
	else {
		ERROR("[DialogueIO]: Trigger too long to fit (200 chars max)");
		return false;
	}

	return true;
}

bool DialogueIO::createNewLink(LinkNode** node, const std::string& line) {
	if (*node != nullptr) {
		ERROR("[DialogueIO]: Cannot start new link node after old one is still open (close with ])");
		return false;
	}
	*node = new LinkNode();
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
	stack.append(tabs(indentation) + "{" + std::to_string(static_cast<int>(node->getType())) + "\n");
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


