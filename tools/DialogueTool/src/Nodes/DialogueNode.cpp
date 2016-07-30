#include "Nodes/DialogueNode.h"

DialogueNode::DialogueNode(int tag) {
	m_tag = tag;
}

int DialogueNode::getTag() const {
	return m_tag;
}

const std::string& DialogueNode::getDescription() {
	if (m_description.empty()) {
		m_description = "[" + std::to_string(m_tag) + "] " + getName();
	}
	return  m_description;
}