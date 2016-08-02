#include "Nodes/ChoiceNode.h"
#include "Nodes/NodeCondition.h"
#include "Nodes/NodeTranslation.h"
#include "ApplicationState.h"
#include "Dialogue.h"
#include <sstream>

ChoiceNode::ChoiceNode() : DialogueNode(G_DIA->generateTag()) {
	m_children.at(0)->translation = new NodeTranslation(m_children.at(0)->getNextTag(), "Choice");
	strcpy(m_children.at(0)->translation->tag, "");
}

ChoiceNode::~ChoiceNode() {
}

std::string ChoiceNode::exportToLua(int indent) const {
	if (m_children.empty()) {
		return "--[[ERROR: no choice set in choice node--]]";
	}

	std::stringstream ss;

	ss << tabs(indent) << "DL:createChoiceNode(" + std::to_string(getTag()) + ")\n";
	
	for (auto child : m_children) {

		if (child->condition == nullptr) {
			ss << tabs(indent) << "DL:addChoice(" + std::to_string(getTag()) + ", \"" + child->translation->tag + "\")";
			ss << " -- "<< child->translation->englishTranslation << "\n";
		}
		else {
			ss << tabs(indent) << "if (" << child->condition->exportToLua() << ") then \n";
			ss << tabs(indent + 1) << "DL:addChoice(" + std::to_string(getTag()) + ", \"" + child->translation->tag + "\")";
			ss << " -- " << child->translation->englishTranslation << "\n";
			ss << "end\n";
		}
	}

	ss << tabs(indent) << "DL:addNode()\n";
	return ss.str();
}

void ChoiceNode::addLinkNode(LinkNode* node) {
	delete node->translation;
	node->translation = new NodeTranslation(node->getNextTag(), "Choice");
	DialogueNode::addLinkNode(node);
}

DialogueNodeType ChoiceNode::getType() const {
	return DialogueNodeType::Choice;
}

float ChoiceNode::getButtonHue() const {
	return 0.12f;
}
