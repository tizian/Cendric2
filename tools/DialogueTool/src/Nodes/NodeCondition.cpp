#include "Nodes/NodeCondition.h"

NodeCondition::NodeCondition(NodeConditionType type) {
	m_type = type;
	strcpy(m_conditionString, "true");
}

NodeCondition::~NodeCondition() {
	delete m_condition;
	delete m_child1;
	delete m_child2;
}

std::string NodeCondition::exportToLua() const {
	return m_conditionString;

	/*if (isLeaf()) {
		return m_condition->exportToLua();
	}

	switch (m_type) {
	case NodeConditionType::And:
		if (m_child1 == nullptr || m_child2 == nullptr) break;
		return "( " + m_child1->exportToLua() + " and " + m_child1->exportToLua() + " )";
	case NodeConditionType::Or:
		if (m_child1 == nullptr || m_child2 == nullptr) break;
		return "( " + m_child1->exportToLua() + " or " + m_child1->exportToLua() + " )";
	case NodeConditionType::Not:
		if (m_child1 == nullptr || m_child2 != nullptr) break;
		return "not " + m_child1->exportToLua();
	default: 
		break;
	}

	return "--[[ ERROR: node condition could not be parsed --]]";*/
}

bool NodeCondition::isLeaf() const {
	return m_type == NodeConditionType::Leaf && m_condition != nullptr;
}

NodeConditionType NodeCondition::getType() const {
	return m_type;
}
