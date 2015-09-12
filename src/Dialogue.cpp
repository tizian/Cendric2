#include "Dialogue.h"
#include "CharacterCore.h"
#include "GUI/DialogueWindow.h"
#include "DialogueFactory.h"

// TODO: create a DialogueData class for our lua script to fill.



Dialogue* Dialogue_new(lua_State* L) {
	const char* name = luaL_checkstring(L, 1);
	int hp = luaL_checknumber(L, 2);
	return new Character(name, hp);
}

int Dialogue_getNPCState(lua_State* L) {
	Dialogue* dialogue = luaW_check<Dialogue>(L, 1);
	lua_pushstring(L, dialogue->getID().c_str());
	return 1;
}

static luaL_Reg Dialogue_table[] = {
	{ NULL, NULL }
};

static luaL_Reg Dialogue_metatable[] = {
	{ "getNPCState", Dialogue_getNPCState },
	{ NULL, NULL }
};

static int luaopen_Dialogue(lua_State* L) {
	luaW_register<Dialogue>(L, "Dialogue", Dialogue_table, Dialogue_metatable);
	return 1;
}

void Dialogue::load(const std::string& id, CharacterCore* core, DialogueWindow* window)
{
	m_id = id;
	m_core = core;
	m_window = window;
	m_nodes.clear();

	// http://pastebin.com/PbMTaM0g
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaopen_Dialogue(L);
	if (luaL_loadfile(L, id.c_str())) 
	{
		g_logger->logError("Dialoge", "Cannot open lua script: " + id);
	}

	lua_pcall(L, 0, 0, 0);
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