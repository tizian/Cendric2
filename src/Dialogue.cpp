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
		for (auto& it : m_currentNode->questStates) {
			m_screen->notifyQuestStateChanged(it.first, it.second);
		}
		if (m_currentNode->goldChanges != 0) {
			m_screen->notifyItemChange("gold", m_currentNode->goldChanges);
		}
		for (auto& it : m_currentNode->questProgress) {
			m_screen->notifyQuestConditionFulfilled(it.first, it.second);
		}
		for (auto& it : m_currentNode->conditionProgress) {
			for (auto& condition : it.second) {
				m_screen->notifyConditionAdded(it.first, condition);
			}
		}
		for (auto& it : m_currentNode->itemChanges) {
			m_screen->notifyItemChange(it.first, it.second);
		}
		for (auto& it : m_currentNode->hints) {
			m_screen->addScreenOverlay(ScreenOverlay::createHintScreenOverlay(it));
		}
		if (!m_currentNode->itemToEquip.empty()) {
			auto bean = g_databaseManager->getItemBean(m_currentNode->itemToEquip);
			if (bean.status == BeanStatus::Filled) {
				m_screen->getCharacterCore()->equipItem(bean.item_id, bean.item_type);
				m_screen->getInventory()->reload();
			}
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