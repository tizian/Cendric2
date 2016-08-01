#include "GUI/NodeWindow.h"
#include "GUI/ConditionWindow.h"
#include "Nodes/DialogueNode.h"
#include "Nodes/StartNode.h"
#include "Nodes/Condition.h"

NodeWindow::NodeWindow() {
}

void NodeWindow::update() {
	if (m_node == nullptr) return;

	ImGui::Begin("Node");
	ImGui::Text(m_node->getDescription().c_str());
	
	switch (m_node->getType()) {
	case DialogueNodeType::Start:
		showStartNodeWindow();
		break;
	case DialogueNodeType::Choice:
		showChoiceNodeWindow();
		break;
	case DialogueNodeType::NPC:
		showNpcNodeWindow();
		break;
	case DialogueNodeType::Trade:
		showTradeNodeWindow();
		break;
	default:
		break;
	}

	ImGui::End();
}

void NodeWindow::setNode(DialogueNode* node) {
	m_node = node;
}

void NodeWindow::showStartNodeWindow() {
	StartNode* sNode = dynamic_cast<StartNode*>(m_node);
	if (sNode == nullptr) return;
	ImGui::InputInt("Default Root Tag", sNode->getDefaultRoot());
	ImGui::Spacing();

	int counter = 0;
	for (auto it = sNode->getRootNodes().begin(); it != sNode->getRootNodes().end(); /*don't increment here*/) {
		// show root node sub window
		ImGui::BeginChild(("Sub" + std::to_string(counter)).c_str(), ImVec2(0, 150), true);
		ImGui::Text(("[" + std::to_string((*it)->nextTag) + "]").c_str());
		ImGui::Combo("", &(*it)->currentPreselectedCondition, Condition::CONDITION_TYPES);
		ImGui::SameLine();
		if (ImGui::Button("Add Condition")) {
			(*it)->addConditionTemplate();
		}
		ImGui::InputTextMultiline("Condition", (*it)->condition->getConditionString(), IM_ARRAYSIZE((*it)->condition->getConditionString()), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 3), ImGuiInputTextFlags_AllowTabInput);
		ImGui::InputInt("Root Tag", &(*it)->nextTag);
		if (ImGui::Button("Remove Root")) {
			delete (*it);
			it = sNode->getRootNodes().erase(it);
		}
		else {
			++it;
		}

		ImGui::EndChild();
		ImGui::Spacing();
		++counter;
	}

	if (ImGui::Button("Add Root")) {
		// adds a new dummy condition
		StartGotoNode* gotoRoot = new StartGotoNode();
		gotoRoot->condition = new NodeCondition(NodeConditionType::Raw);
		sNode->addGotoRoot(gotoRoot);
	}
}

void NodeWindow::showChoiceNodeWindow() {

}

void NodeWindow::showNpcNodeWindow() {
	
}

void NodeWindow::showTradeNodeWindow() {

}
