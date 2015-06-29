#include "MapLoader.h"
#include "MapMainCharacter.h"
#include "Map.h"
#include "NPC.h"

void MapLoader::loadNpcs(MapData& data, Screen* screen) const
{
	MapMainCharacter* mainCharacter = dynamic_cast<MapMainCharacter*>(screen->getObjects(GameObjectType::_MainCharacter)->at(0));
	if (mainCharacter == nullptr)
	{
		g_logger->logError("MapLoader", "Could not find main character of map screen");
		return;
	}

	for (auto& it : data.npcPositions)
	{
		NPCBean npc;
		switch (it.first)
		{
		case NpcID::Guard:
			npc.spriteOffset = sf::Vector2f(0.f, 0.f);
			npc.boundingBox = sf::FloatRect(0, 0, 50, 50);
			npc.texturePositions.push_back(sf::IntRect(0, 0, 50, 50));
			npc.frameTime = sf::seconds(1.0f);
			npc.tooltip = "NPC_Guard";
			npc.talksActive = true;
			break;
		default:
			// unexpected error
			g_logger->logError("MapLoader", "NPC was not loaded, unknown id.");
			return;
		}

		NPC* mapNPC = new NPC();
		Animation idleAnimation;
		mapNPC->setSpriteOffset(npc.spriteOffset);
		mapNPC->setBoundingBox(npc.boundingBox);
		mapNPC->setActiveTalking(npc.talksActive);
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
		mapNPC->setTooltipText(g_textProvider->getText(npc.tooltip));
		screen->addObject(GameObjectType::_NPC, mapNPC);
	}
}