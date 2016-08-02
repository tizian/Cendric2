#pragma once

#include "global.h"
#include "Nodes/DialogueNode.h"

// Trade node, very simple node that allows trading and determines the next node
class TradeNode : public virtual DialogueNode {
public:
	TradeNode();
	~TradeNode();

	std::string exportToLua(int indentationLevel) const override;

	DialogueNodeType getType() const override;
	float getButtonHue() const override;

protected:
	std::string getName() const override { return "Trade Node"; }
};