#include "DialogueLoader.h"
#include "CharacterCore.h"

using namespace std;
using namespace luabridge;

DialogueLoader::DialogueLoader(Dialogue& dialogue, CharacterCore* core) : m_dialogue(dialogue)
{
	m_core = core;
}

DialogueLoader::~DialogueLoader()
{
	delete m_currentNode;
}

void DialogueLoader::loadDialogue()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	getGlobalNamespace(L)
		.beginClass<DialogueLoader>("Dialogue")
		.addFunction("isNPCNeverTalkedTo", &DialogueLoader::isNPCNeverTalkedTo)
		.addFunction("isQuestCompleted", &DialogueLoader::isQuestCompleted)
		.addFunction("setNPCTalking", &DialogueLoader::setNPCTalking)
		.addFunction("setCendricTalking", &DialogueLoader::setCendricTalking)
		.addFunction("setText", &DialogueLoader::setText)
		.addFunction("setNextTag", &DialogueLoader::setNextTag)
		.addFunction("setRoot", &DialogueLoader::setRoot)
		.addFunction("addNode", &DialogueLoader::addNode)
		.endClass();

	if (luaL_dofile(L, m_dialogue.getID().c_str()) != 0) 
	{
		g_logger->logError("DialogeLoader", "Cannot do lua script: " + m_dialogue.getID());
		return;
	}

	LuaRef function = getGlobal(L, "loadDialogue");
	if (!function.isFunction())
	{
		g_logger->logError("DialogeLoader", "Lua script: " + m_dialogue.getID() + " has no loadDialogue function.");
		return;
	}

	try
	{
		function(this);
	}
	catch (LuaException const& e) 
	{
		g_logger->logError("DialogeLoader", "LuaException: " + std::string(e.what()));
		return;
	}


		/*
		switch (dialogue.getID())
		{
		case DialogueID::Guard:
		if (core->getNPCState("npc_guard") == NPCState::Never_talked)
		{
		node = DialogueNode();
		node.isNPCSpeaking = true;
		node.text = "DL_Guard_Hello"; // Hey you, stop! What are you doing here, so very alone? It's dangerous out here.
		node.nextTag = 1;
		dialogue.addNode(0, node);

		node = DialogueNode();
		node.isNPCSpeaking = false;
		node.text = "";
		node.descisions.push_back(pair<string, int>("DL_Choice_GiveMeAQuest", 2)); // Give me a quest.
		node.descisions.push_back(pair<string, int>("DL_Choice_Nothing", -1)); // None of your business.
		dialogue.addNode(1, node);

		node = DialogueNode();
		node.isNPCSpeaking = true;
		node.text = "DL_Guard_KillSomeRats"; // Okay, go and kill some rats for me.
		node.npcProgress.insert({ "npc_guard", NPCState::Passive });
		node.nextTag = 3;
		dialogue.addNode(2, node);

		node = DialogueNode();
		node.isNPCSpeaking = false;
		node.text = "DL_Cendric_Okay"; // Okay, I will do that
		node.questProgress.insert({ QuestID::A_Cave_Of_Rats, QuestState::Started });
		node.nextTag = -1;
		dialogue.addNode(3, node);

		dialogue.setRoot(0);
		}
		else if (core->getQuestState(QuestID::A_Cave_Of_Rats) == QuestState::Started)
		{
		node.isNPCSpeaking = true;
		node.text = "DL_Guard_KillThemAlready"; // You haven't killed those rats! Go and do your job.
		node.nextTag = -1;
		dialogue.addNode(4, node);
		dialogue.setRoot(4);
		}
		break;
		default:
		break;
		}
		*/
}

bool DialogueLoader::isNPCNeverTalkedTo(const std::string& npcID) const
{
	return m_core->getNPCState(npcID) == NPCState::Never_talked;
}

bool DialogueLoader::isQuestCompleted(const std::string& questID) const
{
	return m_core->getQuestState(questID) == QuestState::Completed;
}

void DialogueLoader::setCendricTalking()
{
	if (m_currentNode == nullptr) m_currentNode = new DialogueNode();
	m_currentNode->isNPCSpeaking = false;
}

void DialogueLoader::setNPCTalking()
{
	if (m_currentNode == nullptr) m_currentNode = new DialogueNode();
	m_currentNode->isNPCSpeaking = true;
}

void DialogueLoader::setText(const std::string& text)
{
	if (m_currentNode == nullptr) m_currentNode = new DialogueNode();
	m_currentNode->text = text;
}

void DialogueLoader::setNextTag(int tag)
{
	if (m_currentNode == nullptr) m_currentNode = new DialogueNode();
	m_currentNode->nextTag = tag;
}

void DialogueLoader::addNode(int tag)
{
	if (m_currentNode == nullptr) return;
	m_dialogue.addNode(tag, *m_currentNode);
	delete m_currentNode;
	m_currentNode = nullptr;
}

void DialogueLoader::setRoot(int tag)
{
	m_dialogue.setRoot(tag);
}