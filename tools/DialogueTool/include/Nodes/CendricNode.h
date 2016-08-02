#pragma once

#include "global.h"
#include "Nodes/TriggerableNode.h"

// Cendric Node, it holds a link to the next node, 0 to n triggers and a translation
class CendricNode : public virtual TriggerableNode {
public:
	CendricNode();

	DialogueNodeType getType() const override;
	float getButtonHue() const override;

protected:
	std::string getName() const override { return "Cendric Node"; }
	std::string getTriggerableNodeType() const override { return "Cendric"; }
};