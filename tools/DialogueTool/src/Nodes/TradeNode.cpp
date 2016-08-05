#include "Nodes/TradeNode.h"
#include "ApplicationState.h"
#include "Dialogue.h"
#include <sstream>

TradeNode::TradeNode(int tag) : DialogueNode(tag) {
}

TradeNode::TradeNode() : DialogueNode(G_DIA->generateTag()) {
}

TradeNode::~TradeNode() {
}

std::string TradeNode::exportToLua(int indent) const {
	if (m_children.size() != 1) {
		return "--[[ERROR: trade node must only have exactly one child--]]";
	}

	std::stringstream ss;
	auto child = m_children.at(0);

	ss << tabs(indent) << "DL:createTradeNode(" + std::to_string(getTag()) + ", " + std::to_string(child->getNextTag()) + ")\n";
	ss << tabs(indent) << "DL:addNode()\n";
	return ss.str();
}

DialogueNodeType TradeNode::getType() const {
	return DialogueNodeType::Trade;
}

float TradeNode::getButtonHue() const {
	return 0.31f;
}
