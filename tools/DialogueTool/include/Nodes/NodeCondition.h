#pragma once

#include "global.h"
#include "Condition.h"

#define RAW_CONDITION_LENGTH 2000

enum class NodeConditionType {
	Raw,
	Leaf,
	And,
	Or,
	Not
};

// a node condition, technically a boolean value. A tree over conditions.
class NodeCondition { 
public:
	NodeCondition(NodeConditionType type);
	~NodeCondition();

	std::string exportToLua() const;
	// when a node is a leaf, the condition is filled and both children nodes are null
	bool isLeaf() const;
	NodeConditionType getType() const;
	char(&getConditionString())[RAW_CONDITION_LENGTH] { return m_conditionString; }

private:
	NodeConditionType m_type;
	char m_conditionString[RAW_CONDITION_LENGTH];

	Condition* m_condition = nullptr;
	NodeCondition* m_child1 = nullptr;
	NodeCondition* m_child2 = nullptr;
};