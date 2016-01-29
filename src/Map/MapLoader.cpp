#include "Map/MapLoader.h"
#include "Map/MapMainCharacter.h"
#include "Map/Map.h"
#include "Map/NPC.h"
#include "LightObject.h"
#include "Screens/MapScreen.h"
#include "ObjectFactory.h"

void MapLoader::loadNpcs(MapData& data, Screen* screen, Map* map) const {
	MapMainCharacter* mainCharacter = dynamic_cast<MapScreen*>(screen)->getMainCharacter();
	if (mainCharacter == nullptr) {
		g_logger->logError("MapLoader", "Could not find main character of map screen");
		return;
	}

	// calculate npcs
	for (auto& it : data.npcs) {
		NPC* mapNPC = new NPC(map);
		mapNPC->load(mainCharacter, it);
		screen->addObject(mapNPC);
	}
}

void MapLoader::loadLights(MapData& data, Screen* screen) const {
	// calculate lights
	for (auto& it : data.lights) {
		LightObject* lightObject = new LightObject(it);
		screen->addObject(lightObject);
	}
}

void MapLoader::loadDynamicTiles(MapData& data, Screen* screen, Map* map) const {
	MapMainCharacter* mainCharacter = dynamic_cast<MapScreen*>(screen)->getMainCharacter();
	if (mainCharacter == nullptr) {
		g_logger->logError("MapLoader", "Could not find main character of game screen");
		return;
	}

	for (auto& it : data.dynamicTiles) {
		MapDynamicTile* tile = ObjectFactory::Instance()->createMapDynamicTile(it.id, map);
		if (tile == nullptr) {
			g_logger->logError("MapLoader", "Dynamic tile was not loaded, unknown id.");
			return;
		}
		
		tile->init();
		tile->setPosition(it.position - tile->getPositionOffset());
		tile->setDebugBoundingBox(sf::Color::Yellow);
		tile->load(it.skinNr);
		screen->addObject(tile);
	}
}