#include "GUI/ConditionWindow.h"
#include "Nodes/NodeCondition.h"

void ConditionWindow::showWindow(NodeCondition* condition) {
	// TODO: create some gui representation of a condition
	//ImGui::Combo("Condition Type", &condition->getType(), Condition::CONDITION_TYPES, 4); ImGui::SameLine();
	ImGui::InputText("Condition", condition->getConditionString(), IM_ARRAYSIZE(condition->getConditionString()));
}
