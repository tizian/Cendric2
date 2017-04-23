#pragma once

#include "global.h"
#include "Nodes/DialogueNode.h"

// Trade node, very simple node that allows trading and determines the next node
class TradeNode final : public DialogueNode {
public:
	TradeNode(int tag);
	TradeNode();
	~TradeNode();

	std::string exportToLua(int indentationLevel) const override;

	DialogueNodeType getType() const override;
	float getButtonHue() const override;

protected:
	std::string getName() const override { return "Trade Node"; }
};