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

	// calculate npcs
	for (auto& it : data.npcs)
	{
		NPC* mapNPC = new NPC();
		mapNPC->load(mainCharacter, it);
		screen->addObject(mapNPC);
	}
}