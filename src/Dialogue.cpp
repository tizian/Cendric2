#include "Dialogue.h"
#include "CharacterCore.h"
#include "GUI/DialogueWindow.h"
#include "DialogueLoader.h"

void Dialogue::load(const std::string& id, CharacterCore* core, DialogueWindow* window)
{
	m_id = id;
	m_core = core;
	m_window = window;
	m_nodes.clear();

	DialogueLoader loader(*this, core);
	loader.loadDialogue();
}

void Dialogue::addNode(int tag, const DialogueNode& node)
{
	m_nodes.insert({ tag, node });
}

const std::string& Dialogue::getID() const 
{
	return m_id;
}

bool Dialogue::updateWindow()
{
	if (m_currentNode == nullptr)
	{
		return false;
	}
	if (!m_currentNode->descisions.empty())
	{
		m_window->setDialogueChoice(m_currentNode->descisions);
	}
	else
	{
		if (m_currentNode->isNPCSpeaking)
		{
			m_window->setNPCTalking(m_currentNode->text);
		}
		else
		{
			m_window->setCendricTalking(m_currentNode->text);
		}
	}
	return true;
}

void Dialogue::setRoot(int root)
{
	m_currentNode = &m_nodes[root];
}

void Dialogue::setNextNode(int choice)
{
	if (m_currentNode == nullptr) return;
	int nextNode;
	if (choice == -1)
	{
		nextNode = m_currentNode->nextTag;
	}
	else
	{
		nextNode = m_currentNode->descisions[choice].second;
	}

	if (nextNode == -1)
	{
		m_currentNode = nullptr;
		return;
	}

	m_currentNode = &m_nodes[nextNode];
	for (auto& it : m_currentNode->npcProgress)
	{
		m_core->setNPCState(it.first, it.second);
	}
	for (auto& it : m_currentNode->questProgress)
	{
		m_core->setQuestState(it.first, it.second);
	}
}