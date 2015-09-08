#include "NPCFactory.h"

void NPCFactory::loadNPCBean(NPCBean& npc, const std::string& id)
{
	if (id.compare("npc_guard") == 0)
	{
		npc.spriteOffset = sf::Vector2f(0.f, 0.f);
		npc.boundingBox = sf::FloatRect(0, 0, 50, 50);
		npc.texturePositions.push_back(sf::IntRect(0, 0, 50, 50));
		npc.frameTime = sf::seconds(1.0f);
		npc.name = "npc_guard";
		npc.talksActive = true;
		npc.dialogueID = DialogueID::Guard;
		npc.dialogueTexturePositon = sf::IntRect(250, 0, 250, 250);
	}
}