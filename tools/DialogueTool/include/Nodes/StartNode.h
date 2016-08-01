#pragma once

#include "global.h"
#include "Nodes/DialogueNode.h"
#include "Nodes/NodeCondition.h"

struct StartGotoNode {
	~StartGotoNode();

	// adds the currently selected condition template to the 
	// condition string
	void addConditionTemplate();

	int nextTag = -1;
	int nextType = static_cast<int>(DialogueNodeType::End); 
	NodeCondition* condition = nullptr;
	int currentPreselectedCondition = 0;
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
	std::vector<StartGotoNode*>& getRootNodes();

protected:
	std::string getName() const override { return "Start Node"; }

private:
	std::vector<StartGotoNode*> m_rootNodes;
	int m_defaultRoot = -1;
};