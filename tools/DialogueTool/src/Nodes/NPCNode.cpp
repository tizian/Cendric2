#include "Nodes/NpcNode.h"
#include "Nodes/NodeCondition.h"
#include "Nodes/NodeTranslation.h"
#include "Nodes/NodeTrigger.h"
#include "ApplicationState.h"
#include "Dialogue.h"
#include <sstream>

NpcNode::NpcNode() : DialogueNode(G_DIA->generateTag()) {
	m_translation = new NodeTranslation(getTag(), G_DIA->getNpcID());
}

DialogueNodeType NpcNode::getType() const {
	return DialogueNodeType::NPC;
}

float NpcNode::getButtonHue() const {
	return 0.65f;
}