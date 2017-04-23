#pragma once

#include "global.h"
#include "Nodes/TriggerableNode.h"

// NPC Node, it holds a link to the next node, 0 to n triggers and a translation
class NpcNode final : public TriggerableNode {
public:
	NpcNode(int tag);
	NpcNode();

	DialogueNodeType getType() const override;
	float getButtonHue() const override;

protected:
	std::string getName() const override { return "NPC Node"; }
	std::string getTriggerableNodeType() const override { return "NPC"; }
};