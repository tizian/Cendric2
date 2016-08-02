#include "Nodes/DialogueNode.h"
#include "Nodes/NodeCondition.h"
#include "Nodes/NPCNode.h"
#include "ApplicationState.h"
#include "Dialogue.h"

// Link Node

LinkNode::~LinkNode() {
	delete condition;
	delete nextNode;
}

int LinkNode::getNextTag() {
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
		break;
	case DialogueNodeType::Cendric:
		break;
	case DialogueNodeType::Trade:
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

// DialogueNode

const char* DialogueNode::NODE_TYPES =
"End Dialogue\0NPC Node\0Choice Node\0Cendric Node\0Trade Node\0Reload Dialogue\0\0";

DialogueNode::DialogueNode(int tag) {
	m_tag = tag;
	LinkNode* nextNode = new LinkNode();
	m_children.push_back(nextNode);
}

DialogueNode::~DialogueNode() {
	// recursive cleanup
	G_DIA->freeTag(m_tag);
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