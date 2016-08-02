#include "GUI/TreeWindow.h"
#include "GUI/NodeWindow.h"
#include "ApplicationState.h"
#include "Dialogue.h"
#include "Nodes/StartNode.h"

TreeWindow::TreeWindow(NodeWindow* nodeWindow) {
	m_nodeWindow = nodeWindow;
}

void TreeWindow::recursiveShowNode(DialogueNode* node, int depth) {
	if (node == nullptr) return;

	ImGui::Spacing();
	ImGui::SameLine(depth * 10);

	ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(node->getButtonHue(), 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(node->getButtonHue(), 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(node->getButtonHue(), 0.8f, 0.8f));

	if (ImGui::Button(node->getDescription().c_str())) {
		m_nodeWindow->setNode(node);
	}

	ImGui::PopStyleColor(3);

	for (auto child : node->getLinkNodes()) {
		recursiveShowNode(child->nextNode, depth + 1);
	}
}

void TreeWindow::update() {
	if (G_DIA == nullptr) return;

	ImGui::Begin("Nodes");

	// Start with start node and go depth first search through them
	recursiveShowNode(G_DIA->getStartNode(), 1);

	ImGui::End();
}
