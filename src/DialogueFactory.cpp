#include "DialogueFactory.h"
#include "CharacterCore.h"

using namespace std;

void DialogueFactory::loadDialogue(Dialogue& dialogue, CharacterCore* core)
{
	DialogueNode node;
	switch (dialogue.getID())
	{
	case DialogueID::Guard:
		if (core->getNPCState(NPCID::Guard) == NPCState::Never_talked)
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
			node.npcProgress.insert({ NPCID::Guard, NPCState::Passive });
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