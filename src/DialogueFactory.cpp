#include "DialogueFactory.h"
#include "CharacterCore.h"

using namespace std;

void DialogueFactory::testLua()
{
	// create new Lua state
	lua_State *lua_state;
	lua_state = luaL_newstate();

	// load Lua libraries
	static const luaL_Reg lualibs[] =
	{
		{ "base", luaopen_base },
		{ NULL, NULL }
	};

	const luaL_Reg *lib = lualibs;
	for (; lib->func != NULL; lib++)
	{
		lib->func(lua_state);
		lua_settop(lua_state, 0);
	}

	// run the Lua script
	int ret = luaL_dofile(lua_state, "res/lua/test.lua");

	if (ret != 0){
		printf("Error occurs when calling luaL_dofile() Hint Machine 0x%x\n", ret);
		printf("Error: %s", lua_tostring(lua_state, -1));
	}

	// close the Lua state
	lua_close(lua_state); 
}

void DialogueFactory::loadDialogue(Dialogue& dialogue, CharacterCore* core)
{
	DialogueNode node;
	testLua();
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
}