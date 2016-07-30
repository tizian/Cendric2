#include "Nodes/StartNode.h"
#include "Nodes/NodeCondition.h"
#include <sstream>

StartNode::StartNode() : DialogueNode(-1) {
}

StartNode::~StartNode() {
	for (auto& node : m_rootNodes) {
		delete node;
	}
	m_rootNodes.clear();
}

void StartNode::addGotoRoot(StartGotoNode* node) {
	if (node->condition == nullptr) return;
	m_rootNodes.push_back(node);
}

void StartNode::addDefaultRoot(int defaultRoot) {
	m_defaultRoot = defaultRoot;
}

std::string StartNode::exportToLua(int indent) const {
	if (m_rootNodes.empty()) {
		return tabs(indent) + "DL:setRoot(" + std::to_string(m_defaultRoot) + ")";
	}

	size_t iterator = 0;
	size_t rootCount = m_rootNodes.size();

	std::stringstream ss;

	// first condition
	ss << tabs(indent) << "if (" << m_rootNodes.at(iterator)->condition->exportToLua() << ") then \n";
	ss << tabs(indent + 1) <<"DL:setRoot(" << m_rootNodes.at(iterator)->nextTag << ") \n";

	++iterator;
	if (rootCount > 1) {

		// other conditions
		while (iterator < rootCount - 1) {
			ss << tabs(indent) << "elseif (" << m_rootNodes.at(iterator)->condition->exportToLua() << ") then \n";
			ss << tabs(indent + 1) << "DL:setRoot(" << m_rootNodes.at(iterator)->nextTag << ") \n";

			++iterator;
		}

		// last condition
		ss << tabs(indent) << "else \n";
		ss << tabs(indent + 1) << "DL:setRoot(" << m_rootNodes.at(iterator)->nextTag << ") \n";
	}

	ss << tabs(indent) << "end \n";
	return ss.str();
}

DialogueNodeType StartNode::getType() const { 
	return DialogueNodeType::Start; 
}

int* StartNode::getDefaultRoot() { 
	return &m_defaultRoot; 
}

const std::vector<StartGotoNode*>& StartNode::getRootNodes() const { 
	return m_rootNodes; 
}