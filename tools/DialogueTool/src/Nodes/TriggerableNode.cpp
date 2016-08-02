#include "Nodes/TriggerableNode.h"
#include "Nodes/NodeCondition.h"
#include "Nodes/NodeTranslation.h"
#include "Nodes/NodeTrigger.h"
#include "ApplicationState.h"
#include "Dialogue.h"
#include <sstream>

TriggerableNode::TriggerableNode() : DialogueNode(G_DIA->generateTag()) {
}

TriggerableNode::~TriggerableNode() {
	delete m_translation;
	for (auto it : m_triggers) {
		delete it;
	}
	m_triggers.clear();
}

void TriggerableNode::addTriggerTemplate() {
	TriggerType type = static_cast<TriggerType>(currentPreselectedTrigger + 1);
	if (type <= TriggerType::Raw || type >= TriggerType::MAX) return;
	NodeTrigger* trigger = new NodeTrigger(type);

	std::string templateString = std::string(trigger->exportToLua());
	trigger->type = TriggerType::Raw;
	if (templateString.size() > RAW_TRIGGER_LENGTH) {
		delete trigger;
		return;
	}
	strcpy(trigger->rawTrigger, templateString.c_str());
	m_triggers.push_back(trigger);
}

std::string TriggerableNode::exportToLua(int indent) const {
	if (m_children.size() != 1) {
		return "-- ERROR: " + getTriggerableNodeType() + " node must only have exactly one child\n";
	}

	std::stringstream ss;
	auto child = m_children.at(0);

	ss << tabs(indent) << "DL:create" + getTriggerableNodeType() + "Node(" + std::to_string(getTag()) + ", " + std::to_string(child->getNextTag()) + ", \"" + m_translation->tag + "\")";
	ss << " -- " << m_translation->englishTranslation << "\n";

	for (auto trigger : m_triggers) {
		ss << tabs(indent) << trigger->exportToLua() << "\n";
	}

	ss << tabs(indent) << "DL:addNode()\n";
	return ss.str();
}

std::string TriggerableNode::exportToSQL() const {
	if (m_children.size() != 1) {
		return "-- ERROR: " + getTriggerableNodeType() + " node must only have exactly one child\n";
	}

	std::stringstream ss;
	auto child = m_children.at(0);

	std::string en = duplicateApostrophs(m_translation->englishTranslation);
	std::string de = duplicateApostrophs(m_translation->germanTranslation);
	std::string ch = duplicateApostrophs(m_translation->swissgermanTranslation);
	ss << "INSERT INTO text(text_id, text_type, english, german, swiss_german) values ('";
	ss << m_translation->tag << "', '" << G_DIA->getNpcID() << "', '" << en + "', '" + de + "', '" + ch + "');\n";

	return ss.str();
}

NodeTranslation* TriggerableNode::getTranslation() {
	return m_translation;
}

std::vector<NodeTrigger*>& TriggerableNode::getTriggers() {
	return m_triggers;
}
