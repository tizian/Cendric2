#include "GUI/TreeWindow.h"
#include "GUI/NodeWindow.h"
#include "ApplicationState.h"
#include "Dialogue.h"
#include "Nodes/StartNode.h"

TreeWindow::TreeWindow(NodeWindow* nodeWindow) {
	m_nodeWindow = nodeWindow;
}

void TreeWindow::update() {
	if (G_DIA == nullptr) return;

	ImGui::Begin("Nodes");
	if (ImGui::Button(G_DIA->getStartNode()->getDescription().c_str())) {
		m_nodeWindow->setNode(G_DIA->getStartNode());
	}


	ImGui::End();

}
