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

	int it = 0;
	for (auto rootNode : sNode->getRootNodes()) {
		// show root node sub window
		ImGui::BeginChild(("Sub" + std::to_string(it)).c_str(), ImVec2(0, 100), true);
        ImGui::Text(("[" + std::to_string(rootNode->nextTag) + "]").c_str());
		ImGui::InputTextMultiline("Condition", rootNode->condition->getConditionString(), IM_ARRAYSIZE(rootNode->condition->getConditionString()), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 3), ImGuiInputTextFlags_AllowTabInput);
		ImGui::InputInt("Root Tag", &rootNode->nextTag);
		ImGui::EndChild();
		ImGui::Spacing();
		++it;
	}

	if (ImGui::Button("Add Root")) {
		// adds a new dummy condition
		StartGotoNode* gotoRoot = new StartGotoNode();
		gotoRoot->condition = new NodeCondition(NodeConditionType::Direct);
		sNode->addGotoRoot(gotoRoot);
	}
}

void NodeWindow::showChoiceNodeWindow() {

}

void NodeWindow::showNpcNodeWindow() {
	
}

void NodeWindow::showTradeNodeWindow() {

}
