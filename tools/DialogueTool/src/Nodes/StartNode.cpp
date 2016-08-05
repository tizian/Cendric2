#include "Nodes/StartNode.h"
#include "Nodes/NodeCondition.h"
#include "ApplicationState.h"
#include <sstream>

StartNode::StartNode(int tag) : DialogueNode(tag) {
}

StartNode::StartNode() : DialogueNode(-1) {
}

StartNode::~StartNode() {
}

std::string StartNode::exportToLua(int indent) const {
	if (m_children.empty()) {
		return "--[[ERROR: no root node set in start node--]]";
	}

	// check if exactly one child has no condition (the else)
	int noCondition = 0;
	LinkNode* elseNode = nullptr;
	for (auto it : m_children) {
		if (it->condition == nullptr) {
			++noCondition;
			elseNode = it;
		}
	}

	if (noCondition != 1) {
		ERROR("[Start Node Export]: exactly one root node must have no condition! (default case)");
		return "--[[ERROR: exactly one root node must have no condition! (default case)--]]";
	}

	// re-order children
	std::vector<LinkNode*> nodes = m_children;
	nodes.erase(std::find(nodes.begin(), nodes.end(), elseNode));
	nodes.push_back(elseNode);
	
	size_t iterator = 0;
	size_t rootCount = nodes.size();

	std::stringstream ss;

	// only one root
	if (rootCount == 1) {
		ss << tabs(indent) << "DL:setRoot(" << elseNode->getNextTag() << ") \n";
		return ss.str();
	} 

	// first condition
	ss << tabs(indent) << "if (" << nodes.at(iterator)->condition->exportToLua() << ") then \n";
	ss << tabs(indent + 1) <<"DL:setRoot(" << nodes.at(iterator)->getNextTag() << ") \n";

	++iterator;
	if (rootCount > 1) {

		// other conditions
		while (iterator < rootCount - 1) {
			ss << tabs(indent) << "elseif (" << nodes.at(iterator)->condition->exportToLua() << ") then \n";
			ss << tabs(indent + 1) << "DL:setRoot(" << nodes.at(iterator)->getNextTag() << ") \n";

			++iterator;
		}

		// last node (has no condition)
		ss << tabs(indent) << "else \n";
		ss << tabs(indent + 1) << "DL:setRoot(" << nodes.at(iterator)->getNextTag() << ") \n";
	}

	ss << tabs(indent) << "end \n";
	return ss.str();
}

DialogueNodeType StartNode::getType() const { 
	return DialogueNodeType::Start; 
}

float StartNode::getButtonHue() const {
	return 0.0f;
}
