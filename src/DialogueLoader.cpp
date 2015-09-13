#include "DialogueLoader.h"
#include "CharacterCore.h"

using namespace std;
using namespace luabridge;

DialogueLoader::DialogueLoader(Dialogue& dialogue, CharacterCore* core) : m_dialogue(dialogue)
{
	m_core = core;
}

void DialogueLoader::loadDialogue()
{
	DialogueNode node;

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	if (luaL_loadfile(L, m_dialogue.getID().c_str()))
	{
		g_logger->logError("Dialoge", "Cannot open lua script: " + m_dialogue.getID());
		return;
	}

	lua_pcall(L, 0, 0, 0);

	LuaRef s = getGlobal(L, "testString");
	LuaRef n = getGlobal(L, "number");
	std::string luaString = s.cast<std::string>();
	int answer = n.cast<int>();
	std::cout << luaString << std::endl;
	std::cout << "And here's our number:" << answer << std::endl;

	LuaRef t = getGlobal(L, "window");
	LuaRef title = t["title"];
	LuaRef w = t["width"];
	LuaRef h = t["height"];
	std::string titleString = title.cast<std::string>();
	int width = w.cast<int>();
	int height = h.cast<int>();
	std::cout << titleString << std::endl;
	std::cout << "width = " << width << std::endl;
	std::cout << "height = " << height << std::endl;
	
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