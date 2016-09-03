#include "Nodes/DialogueNode.h"
#include "Nodes/NodeCondition.h"
#include "Nodes/NPCNode.h"
#include "Nodes/CendricNode.h"
#include "Nodes/TradeNode.h"
#include "Nodes/ChoiceNode.h"
#include "Nodes/NodeTranslation.h"
#include "ApplicationState.h"
#include "Dialogue.h"
#include "FileIO/DialogueIO.h"

#include <sstream>

// Link Node

LinkNode::~LinkNode() {
	delete condition;
	delete nextNode;
	delete translation;
}

int LinkNode::getNextTag() const {
	if (nextNode == nullptr) {
		return isReload ? -2 : -1;
	}
	return nextNode->getTag();
}

void LinkNode::addConditionTemplate() {
	if (condition == nullptr) {
		condition = new NodeCondition(NodeConditionType::Raw);
	}
	ConditionType type = static_cast<ConditionType>(currentPreselectedCondition + 1);
	if (type <= ConditionType::VOID || type >= ConditionType::MAX) return;
	Condition cond;
	cond.type = type;
	std::string templateString = std::string(condition->getConditionString()) + cond.exportToLua();
	if (templateString.size() > RAW_CONDITION_LENGTH) {
		return;
	}
	strcpy(condition->getConditionString(), templateString.c_str());
}

void LinkNode::linkNodeTemplate() {
	DialogueNodeType type = static_cast<DialogueNodeType>(currentPreselectedNodetype);
	if (type < DialogueNodeType::End || type > DialogueNodeType::Start) return;

	switch (type) {

	case DialogueNodeType::Start:
		delete nextNode;
		nextNode = nullptr;
		isReload = true;
		break;
	case DialogueNodeType::NPC:
		delete nextNode;
		nextNode = new NpcNode();
		break;
	case DialogueNodeType::Choice:
		delete nextNode;
		nextNode = new ChoiceNode();
		break;
	case DialogueNodeType::Cendric:
		delete nextNode;
		nextNode = new CendricNode();
		break;
	case DialogueNodeType::Trade:
		delete nextNode;
		nextNode = new TradeNode();
		break;
	case DialogueNodeType::End:
		delete nextNode;
		nextNode = nullptr;
		isReload = false;
		break;
	default:
		break;
	}
}

std::string LinkNode::exportToDia(int indentationLevel) const {
	std::string diaExport;
	diaExport.append(tabs(indentationLevel) + "# Link node, tag is next tag\n");
	diaExport.append(tabs(indentationLevel) + DialogueIO::TAG + ":" + std::to_string(getNextTag()) + "\n");
	if (condition != nullptr) {
		diaExport.append(tabs(indentationLevel) + DialogueIO::CONDITION + ":" + replaceNewlines(condition->getConditionString()) + "\n");
	}
	if (translation != nullptr) {
		diaExport.append(tabs(indentationLevel) + "# translation: \n");
		diaExport.append(tabs(indentationLevel) + DialogueIO::TRANSLATION_TAG + ":" + replaceNewlines(translation->tag) + "\n");
		diaExport.append(tabs(indentationLevel) + DialogueIO::TRANSLATION_EN + ":" + replaceNewlines(translation->englishTranslation) + "\n");
		diaExport.append(tabs(indentationLevel) + DialogueIO::TRANSLATION_DE + ":" + replaceNewlines(translation->germanTranslation) + "\n");
		diaExport.append(tabs(indentationLevel) + DialogueIO::TRANSLATION_CH + ":" + replaceNewlines(translation->swissgermanTranslation) + "\n");

		if (translation->itemAmount > 0) {
			diaExport.append(tabs(indentationLevel) + DialogueIO::TRANSLATION_ITEM_ID + ":" + translation->itemID + "\n");
			diaExport.append(tabs(indentationLevel) + DialogueIO::TRANSLATION_ITEM_AMOUNT + ":" + std::to_string(translation->itemAmount) + "\n");
		}
	}
	return diaExport;
}

std::string LinkNode::exportToSQL() const {
	if (translation == nullptr) return "";
	if (std::string(translation->englishTranslation).empty()) return "";
	std::stringstream ss;

	std::string en = duplicateApostrophs(translation->englishTranslation);
	std::string de = duplicateApostrophs(translation->germanTranslation);
	std::string ch = duplicateApostrophs(translation->swissgermanTranslation);
	ss << "INSERT INTO text(text_id, text_type, english, german, swiss_german) values ('";
	ss << translation->tag << "', 'dl_" << G_DIA->getNpcID() << "', '" << en + "', '" + de + "', '" + ch + "');\n";

	return ss.str();
}

// DialogueNode

const char* DialogueNode::NODE_TYPES =
"End Dialogue\0NPC Node\0Choice Node\0Cendric Node\0Trade Node\0Reload Dialogue\0\0";

DialogueNode::DialogueNode(int tag) {
	m_tag = tag;
	if (tag != 0) {
		LinkNode* nextNode = new LinkNode();
		m_children.push_back(nextNode);
	}
}

DialogueNode::~DialogueNode() {
	// recursive cleanup
	if (G_DIA != nullptr) G_DIA->freeTag(m_tag);
	for (auto& node : m_children) {
		delete node;
	}
	m_children.clear();
}

int DialogueNode::getTag() const {
	return m_tag;
}

std::vector<LinkNode*>& DialogueNode::getLinkNodes() {
	return m_children;
}

void DialogueNode::addLinkNode(LinkNode* node) {
	m_children.push_back(node);
}

const std::string& DialogueNode::getDescription() {
	if (m_description.empty()) {
		m_description = "[" + std::to_string(m_tag) + "] " + getName();
	}
	return  m_description;
}

std::string DialogueNode::exportToDia(int indentationLevel) {
	std::string diaExport;
	diaExport.append(tabs(indentationLevel) + "# " + getDescription() + "\n");
	diaExport.append(tabs(indentationLevel) + DialogueIO::TAG + ":" + std::to_string(m_tag) + "\n");
	return diaExport;
}
