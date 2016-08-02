#include "GUI/NodeWindow.h"
#include "GUI/TranslationWindow.h"
#include "Nodes/DialogueNode.h"
#include "Nodes/StartNode.h"
#include "Nodes/TriggerableNode.h"
#include "Nodes/TradeNode.h"
#include "Nodes/ChoiceNode.h"
#include "Nodes/Condition.h"
#include "Nodes/NodeTrigger.h"

NodeWindow::NodeWindow(TranslationWindow* translationWindow) {
	m_translationWindow = translationWindow;
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
	case DialogueNodeType::Cendric:
		showTriggerableNode();
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
	if (m_node == nullptr) return;
	switch (m_node->getType()) {
	case DialogueNodeType::Start:
	case DialogueNodeType::Trade:
	default:
		m_translationWindow->setNodeTranslation(nullptr);
		break;
	case DialogueNodeType::NPC:
	case DialogueNodeType::Cendric:
		m_translationWindow->setNodeTranslation(dynamic_cast<TriggerableNode*>(m_node)->getTranslation());
		break;
	case DialogueNodeType::Choice:
		m_translationWindow->setNodeTranslation(m_node->getLinkNodes().at(0)->translation);
		break;
	}
}

void NodeWindow::showStartNodeWindow() {
	StartNode* sNode = dynamic_cast<StartNode*>(m_node);
	if (sNode == nullptr) return;

	int counter = 0;
	const bool moreThanOneNode = sNode->getLinkNodes().size() > 1;
	for (auto it = sNode->getLinkNodes().begin(); it != sNode->getLinkNodes().end(); /*don't increment here*/) {
		
		// calculate sub window size
		const bool hasCondition = (*it)->condition != nullptr;
		float size = 80;
		if (moreThanOneNode || hasCondition) size += 20.f;
		if (hasCondition) size += 40.f;
		// show root node sub window
		ImGui::BeginChild(("Sub" + std::to_string(counter)).c_str(), ImVec2(0, size), true);
		ImGui::Text(("Next Tag: [" + std::to_string((*it)->getNextTag()) + "]").c_str());
		ImGui::Combo("", &(*it)->currentPreselectedCondition, Condition::CONDITION_TYPES);
		ImGui::SameLine();
		if (ImGui::Button("Add Condition")) {
			(*it)->addConditionTemplate();
		}
		if (hasCondition) {
			ImGui::InputTextMultiline("Condition", (*it)->condition->getConditionString(), IM_ARRAYSIZE((*it)->condition->getConditionString()), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 3), ImGuiInputTextFlags_AllowTabInput);
		}
		if (ImGui::Combo("Next Node", &(*it)->currentPreselectedNodetype, DialogueNode::NODE_TYPES)) {
			(*it)->linkNodeTemplate();
		}

		if (moreThanOneNode && ImGui::Button("Remove Root")) {
			delete (*it);
			it = sNode->getLinkNodes().erase(it);
		}
		else {
			if (hasCondition) {
				if (moreThanOneNode) ImGui::SameLine();
				if (ImGui::Button("Remove Condition")) {
					delete (*it)->condition;
					(*it)->condition = nullptr;
				}
			}

			++it;
		}

		ImGui::EndChild();
		ImGui::Spacing();
		++counter;
	}

	if (ImGui::Button("Add Root")) {
		// adds a new dummy condition
		LinkNode* linkNode = new LinkNode();
		sNode->addLinkNode(linkNode);
	}
}

void NodeWindow::showChoiceNodeWindow() {
	ChoiceNode* cNode = dynamic_cast<ChoiceNode*>(m_node);
	if (cNode == nullptr) return;

	int counter = 0;
	const bool moreThanOneNode = cNode->getLinkNodes().size() > 1;
	for (auto it = cNode->getLinkNodes().begin(); it != cNode->getLinkNodes().end(); /*don't increment here*/) {

		// calculate sub window size
		const bool hasCondition = (*it)->condition != nullptr;
		float size = 100;
		if (moreThanOneNode || hasCondition) size += 20.f;
		if (hasCondition) size += 40.f;
		// show root node sub window
		ImGui::BeginChild(("Sub" + std::to_string(counter)).c_str(), ImVec2(0, size), true);
		ImGui::Text(("Next Tag: [" + std::to_string((*it)->getNextTag()) + "]").c_str());
		if (ImGui::Button("Translation")) {
			m_translationWindow->setNodeTranslation((*it)->translation);
		}
		ImGui::Combo("", &(*it)->currentPreselectedCondition, Condition::CONDITION_TYPES);
		ImGui::SameLine();
		if (ImGui::Button("Add Condition")) {
			(*it)->addConditionTemplate();
		}
		if (hasCondition) {
			ImGui::InputTextMultiline("Condition", (*it)->condition->getConditionString(), IM_ARRAYSIZE((*it)->condition->getConditionString()), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 3), ImGuiInputTextFlags_AllowTabInput);
		}
		if (ImGui::Combo("Next Node", &(*it)->currentPreselectedNodetype, DialogueNode::NODE_TYPES)) {
			(*it)->linkNodeTemplate();
		}

		if (moreThanOneNode && ImGui::Button("Remove Choice")) {
			delete (*it);
			it = cNode->getLinkNodes().erase(it);
		}
		else {
			if (hasCondition) {
				if (moreThanOneNode) ImGui::SameLine();
				if (ImGui::Button("Remove Condition")) {
					delete (*it)->condition;
					(*it)->condition = nullptr;
				}
			}
			
			++it;
		}

		ImGui::EndChild();
		ImGui::Spacing();
		++counter;
	}

	if (ImGui::Button("Add Choice")) {
		// adds a new dummy condition
		LinkNode* linkNode = new LinkNode();
		cNode->addLinkNode(linkNode);
	}
}

void NodeWindow::showTriggerableNode() {
	TriggerableNode* nNode = dynamic_cast<TriggerableNode*>(m_node);
	if (nNode == nullptr || nNode->getLinkNodes().size() != 1) return;
	auto child = nNode->getLinkNodes().at(0);

	ImGui::Text(("Next Tag: [" + std::to_string(child->getNextTag()) + "]").c_str());
	if (ImGui::Combo("Next Node", &child->currentPreselectedNodetype, DialogueNode::NODE_TYPES)) {
		child->linkNodeTemplate();
	}

	ImGui::Spacing();
	ImGui::Text("Triggers");
	ImGui::Combo("", &nNode->currentPreselectedTrigger, NodeTrigger::TRIGGER_TYPES);
	ImGui::SameLine();
	if (ImGui::Button("Add Trigger")) {
		nNode->addTriggerTemplate();
	}

	int counter = 0;
	for (auto it = nNode->getTriggers().begin(); it != nNode->getTriggers().end(); /*don't increment here*/) {
		// show root node sub window
		ImGui::PushID(counter);
		ImGui::InputText("", (*it)->rawTrigger, IM_ARRAYSIZE((*it)->rawTrigger));
	
		ImGui::SameLine();
		if (ImGui::Button("Remove Trigger")) {
			delete (*it);
			it = nNode->getTriggers().erase(it);
		}
		else {
			++it;
		}

		ImGui::PopID();
		++counter;
	}
}

void NodeWindow::showTradeNodeWindow() {
	TradeNode* tNode = dynamic_cast<TradeNode*>(m_node);
	if (tNode == nullptr || tNode->getLinkNodes().size() != 1) return;
	auto child = tNode->getLinkNodes().at(0);

	ImGui::Text(("Next Tag: [" + std::to_string(child->getNextTag()) + "]").c_str());
	if (ImGui::Combo("Next Node", &child->currentPreselectedNodetype, DialogueNode::NODE_TYPES)) {
		child->linkNodeTemplate();
	}
}
