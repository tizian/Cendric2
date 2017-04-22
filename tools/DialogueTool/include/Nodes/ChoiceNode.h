#pragma once

#include "global.h"
#include "Nodes/DialogueNode.h"
#include "Nodes/NodeCondition.h"

// Choice Node, splits the flow into multiple children
class ChoiceNode final : public virtual DialogueNode {
public:
	ChoiceNode(int tag);
	ChoiceNode();
	~ChoiceNode();

	std::string exportToLua(int indentationLevel) const override;
	void addLinkNode(LinkNode* node) override;

	DialogueNodeType getType() const override;
	float getButtonHue() const override;

protected:
	std::string getName() const override { return "Choice Node"; }
};