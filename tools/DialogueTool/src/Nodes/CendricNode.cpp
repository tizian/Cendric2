#include "Nodes/CendricNode.h"
#include "Nodes/NodeCondition.h"
#include "Nodes/NodeTranslation.h"
#include "Nodes/NodeTrigger.h"
#include "ApplicationState.h"
#include "Dialogue.h"
#include <sstream>

CendricNode::CendricNode(int tag) : DialogueNode(tag) {
	m_translation = new NodeTranslation(getTag(), "Cendric");
}

CendricNode::CendricNode() : DialogueNode(G_DIA->generateTag()) {
	m_translation = new NodeTranslation(getTag(), "Cendric");
}

DialogueNodeType CendricNode::getType() const {
	return DialogueNodeType::Cendric;
}

float CendricNode::getButtonHue() const {
	return 0.78f;
}
