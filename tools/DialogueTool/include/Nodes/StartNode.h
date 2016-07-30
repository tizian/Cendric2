#pragma once

#include "global.h"
#include "Nodes/DialogueNode.h"
#include "Nodes/NodeCondition.h"

struct StartGotoNode {
	~StartGotoNode() { delete condition; }

	int nextTag = -1;
	NodeCondition* condition = nullptr;
};

// Start Node, it determines the root of the dialogue
class StartNode : public virtual DialogueNode {
public:
	StartNode();
	~StartNode();

	void addDefaultRoot(int defaultRoot);
	void addGotoRoot(StartGotoNode* node);

	std::string exportToLua(int indentationLevel) const override;

	// getters
	DialogueNodeType getType() const override;
	int* getDefaultRoot();
	const std::vector<StartGotoNode*>& getRootNodes() const;

protected:
	std::string getName() const override { return "Start Node"; }

private:
	std::vector<StartGotoNode*> m_rootNodes;
	int m_defaultRoot = -1;
};