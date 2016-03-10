#include "Dialogue.h"
#include "Screens/WorldScreen.h"
#include "GUI/DialogueWindow.h"
#include "DialogueLoader.h"
#include "Map/NPC.h"

void Dialogue::load(const std::string& id, WorldScreen* screen, DialogueWindow* window) {
	m_id = id;
	m_screen = screen;
	m_window = window;
	m_nodes.clear();

	DialogueLoader loader(*this, screen->getCharacterCore());
	loader.loadDialogue();
}

void Dialogue::addNode(int tag, const DialogueNode& node) {
	if (m_nodes.find(tag) != m_nodes.end()) {
		g_logger->logWarning("Dialogue", "Node with tag [" + std::to_string(tag) + "] already exists in the dialogoue tree.");
		return;
	}
	m_nodes.insert({ tag, node });
}

const std::string& Dialogue::getID() const {
	return m_id;
}

bool Dialogue::updateWindow() {
	if (m_currentNode == nullptr) {
		m_window->getNPC()->reloadRoutine();
		return false;
	}
	if (m_currentNode->type == DialogueNodeType::Choice) {
		m_window->setDialogueChoice(m_currentNode->choices);
	}
	else if (m_currentNode->type == DialogueNodeType::NPCTalking) {
		m_window->setNPCTalking(m_currentNode->text);
	}
	else if (m_currentNode->type == DialogueNodeType::Trade) {
		m_window->setNPCTrading(m_currentNode->text);
	}
	else {
		m_window->setCendricTalking(m_currentNode->text);
	}

	if (m_currentNode != nullptr) {
		for (auto& content : m_currentNode->content) {
			TriggerContent::executeTrigger(content, m_screen);
		}
	}
		
	return true;
}

void Dialogue::setRoot(int root) {
	if (m_nodes.find(root) == m_nodes.end()) {
		g_logger->logWarning("Dialogue", "Node with tag [" + std::to_string(root) + "] does not exist in the tree and cannot be set as root.");
		return;
	}
	m_currentNode = &m_nodes[root];
}

bool Dialogue::isEndable() {
	if (m_currentNode == nullptr) return true;

	if (m_currentNode->choices.empty() && m_currentNode->nextTag == -1) {
		return true;
	}
	for (auto& choice : m_currentNode->choices) {
		if (choice.second == -1) {
			return true;
		}
	}

	return false;
}

void Dialogue::setNextNode(int choice) {
	if (m_currentNode == nullptr) return;
	int nextNode;
	if (choice == -1) {
		nextNode = m_currentNode->nextTag;
	}
	else {
		nextNode = m_currentNode->choices[choice].second;
	}

	if (nextNode == -1) {
		m_currentNode = nullptr;
		return;
	}

	m_currentNode = &m_nodes[nextNode];
}