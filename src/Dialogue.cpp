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
			switch (content.type) {
			case DialogueNodeContentType::ConditionProgress:
				m_screen->notifyConditionAdded(content.firstStringAttribute, content.secondStringAttribute);
				break;
			case DialogueNodeContentType::GoldChange:
				m_screen->notifyItemChange("gold", content.integerAttribute);
				break;
			case DialogueNodeContentType::ItemChange:
				m_screen->notifyItemChange(content.firstStringAttribute, content.integerAttribute);
				break;
			case DialogueNodeContentType::Hint:
				m_screen->addScreenOverlay(ScreenOverlay::createHintScreenOverlay(content.firstStringAttribute));
				break;
			case DialogueNodeContentType::ItemEquip: {
				auto bean = g_databaseManager->getItemBean(content.firstStringAttribute);
				if (bean.status == BeanStatus::Filled) {
					m_screen->getCharacterCore()->equipItem(bean.item_id, bean.item_type);
					m_screen->getInventory()->reload();
				}
				break;
			}
			case DialogueNodeContentType::QuestConditionProgress:
				m_screen->notifyQuestConditionFulfilled(content.firstStringAttribute, content.secondStringAttribute);
				break;
			case DialogueNodeContentType::QuestDescriptionProgress:
				m_screen->notifyQuestDescriptionAdded(content.firstStringAttribute, content.integerAttribute);
				break;
			case DialogueNodeContentType::QuestStateChange:
				m_screen->notifyQuestStateChanged(content.firstStringAttribute, static_cast<QuestState>(content.integerAttribute));
				break;
			default:
				break;
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

bool Dialogue::isEndable() {
	if (m_currentNode == nullptr) return true;

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