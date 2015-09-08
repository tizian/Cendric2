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

	int x = 0;
	int y = 0;

	int tileWidth = data.tileSize.x;
	int tileHeight = data.tileSize.y;

	// calculate npcs
	for (auto& it : data.npcs)
	{
		if (!it.empty())
		{
			sf::Vector2f pos = sf::Vector2f(static_cast<float>(x * tileWidth), static_cast<float>(y * tileHeight));

			NPCBean npc = DEFAULT_NPC;
			factory.loadNPCBean(npc, it);

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
			mapNPC->load(mainCharacter, it);
			mapNPC->setPosition(pos - mapNPC->getSpriteOffset());
			mapNPC->setTooltipText(g_textProvider->getText(npc.name));
			screen->addObject(mapNPC);
		}
		if (x + 1 >= data.mapSize.x)
		{
			x = 0;
			y++;
		}
		else
		{
			x++;
		}
	}

	for (auto& it : data.npcPositions)
	{
		
	}
}