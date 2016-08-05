#pragma once

#include "global.h"
#include "Nodes/DialogueNode.h"
#include "Nodes/NodeCondition.h"

// Start Node, it determines the root of the dialogue
class StartNode : public virtual DialogueNode {
public:
	StartNode(int tag);
	StartNode();
	~StartNode();

	std::string exportToLua(int indentationLevel) const override;

	DialogueNodeType getType() const override;
	float getButtonHue() const override;

protected:
	std::string getName() const override { return "Start Node"; }
};