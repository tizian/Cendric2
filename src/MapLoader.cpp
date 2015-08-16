#include "MapLoader.h"
#include "MapMainCharacter.h"
#include "Map.h"
#include "NPC.h"

void MapLoader::loadNpcs(MapData& data, Screen* screen) const
{
	NPCFactory factory;
	MapMainCharacter* mainCharacter = dynamic_cast<MapMainCharacter*>(screen->getObjects(GameObjectType::_MainCharacter)->at(0));
	if (mainCharacter == nullptr)
	{
		g_logger->logError("MapLoader", "Could not find main character of map screen");
		return;
	}

	for (auto& it : data.npcPositions)
	{
		NPCBean npc = DEFAULT_NPC;
		factory.loadNPCBean(npc, it.first);

		NPC* mapNPC = new NPC();
		Animation idleAnimation;
		mapNPC->setSpriteOffset(npc.spriteOffset);
		mapNPC->setBoundingBox(npc.boundingBox);
		mapNPC->setTalksActive(npc.talksActive);
		mapNPC->setDialogueID(npc.dialogueID);
		idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_npcs));
		// add frames
		for (auto &frame : npc.texturePositions) {
			idleAnimation.addFrame(frame);
		}
		mapNPC->addAnimation(GameObjectState::Idle, idleAnimation);
		mapNPC->setFrameTime(npc.frameTime);
		// initial values
		mapNPC->setCurrentAnimation(mapNPC->getAnimation(GameObjectState::Idle), false);
		mapNPC->playCurrentAnimation(npc.texturePositions.size() > 1);
		mapNPC->load(mainCharacter, it.first);
		mapNPC->setPosition(it.second - mapNPC->getSpriteOffset());
		mapNPC->setTooltipText(g_textProvider->getText(npc.name));
		screen->addObject(mapNPC);
	}
}