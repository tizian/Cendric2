#include "Map/MapLoader.h"
#include "Map/MapMainCharacter.h"
#include "Map/Map.h"
#include "Map/NPC.h"
#include "World/LightObject.h"
#include "Screens/MapScreen.h"
#include "ObjectFactory.h"
#include "World/Trigger.h"

void MapLoader::loadForRenderTexture(MapData& data, MapScreen* screen) {
	loadLights(data, screen);
	loadDynamicTiles(data, screen);
}

void MapLoader::loadAfterMainChar(MapData& data, MapScreen* screen) {
	loadNpcs(data, screen);
	loadTriggers(data, screen);
}

void MapLoader::loadNpcs(MapData& data, MapScreen* screen) {
	// calculate npcs
	for (auto& it : data.npcs) {
		NPC* mapNPC = new NPC(screen);
		mapNPC->load(it);
		screen->addObject(mapNPC);
	}
}

void MapLoader::loadLights(MapData& data, MapScreen* screen) {
	// calculate lights
	for (auto& it : data.lights) {
		LightObject* lightObject = new LightObject(it);
		screen->addObject(lightObject);
	}
}

void MapLoader::loadTriggers(MapData& data, MapScreen* screen) {
	for (auto& it : data.triggers) {
		if (screen->getCharacterCore()->isTriggerTriggered(it.worldID, it.objectID))
			continue;
		Trigger* trigger = new Trigger(screen, it);
		screen->reloadTrigger(trigger);
		screen->addObject(trigger);
	}
}

void MapLoader::loadDynamicTiles(MapData& data, MapScreen* screen) {
	for (auto& it : data.dynamicTiles) {
		MapDynamicTile* tile = ObjectFactory::Instance()->createMapDynamicTile(it.id, screen);
		if (tile == nullptr) {
			g_logger->logError("MapLoader", "Dynamic tile was not loaded, id not registered:" + std::to_string(static_cast<int>(it.id)));
			continue;
		}

		tile->setObjectID(it.objectID);
		tile->loadResources();
		if (!tile->init(it.properties)) {
			g_logger->logError("MapLoader", "Dynamic tile was not loaded, initialization failed.");
			delete tile;
			continue;
		}

		tile->setPosition(it.position + tile->getPositionOffset());
		tile->setDebugBoundingBox(COLOR_NEUTRAL);
		tile->loadAnimation(it.skinNr);

		screen->addObject(tile);
	}
}