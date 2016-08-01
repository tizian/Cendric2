#include "Nodes/DialogueNode.h"

const char* DialogueNode::NODE_TYPES =
"Start Node\0NPC Node\0Choice Node\0Cendric Node\0Trade Node\0End Dialogue\0\0";

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