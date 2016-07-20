#include "Map/MapLoader.h"
#include "Map/MapMainCharacter.h"
#include "Map/Map.h"
#include "Map/NPC.h"
#include "LightObject.h"
#include "Screens/MapScreen.h"
#include "ObjectFactory.h"
#include "Map/DynamicTiles/WaypointTile.h"
#include "Map/DynamicTiles/BookTile.h"
#include "Map/DynamicTiles/SignMapTile.h"
#include "Trigger.h"

void MapLoader::loadNpcs(MapData& data, MapScreen* screen) const {
	// calculate npcs
	for (auto& it : data.npcs) {
		NPC* mapNPC = new NPC(screen);
		mapNPC->load(it);
		screen->addObject(mapNPC);
	}
}

void MapLoader::loadBooks(MapData& data, MapScreen* screen) const {
	// calculate books
	for (auto& it : data.books) {
		BookTile* book = new BookTile(it, screen);
		book->init();
		book->setPosition(it.position + book->getPositionOffset());
		book->setDebugBoundingBox(COLOR_NEUTRAL);
		book->loadResources();
		book->loadAnimation(it.skinNr);

		screen->addObject(book);
	}
}

void MapLoader::loadSigns(MapData& data, MapScreen* screen) const {
	// calculate signs
	for (auto& it : data.signs) {
		SignMapTile* sign = new SignMapTile(it, screen);
		sign->init();
		sign->setPosition(it.position + sign->getPositionOffset());
		sign->setDebugBoundingBox(COLOR_NEUTRAL);
		sign->loadResources();
		sign->loadAnimation(it.skinNr);

		screen->addObject(sign);
	}
}

void MapLoader::loadLights(MapData& data, MapScreen* screen) const {
	// calculate lights
	for (auto& it : data.lights) {
		LightObject* lightObject = new LightObject(it);
		screen->addObject(lightObject);
	}
}

void MapLoader::loadTriggers(MapData& data, MapScreen* screen) const {
	for (auto& it : data.triggers) {
		if (screen->getCharacterCore()->isTriggerTriggered(it.worldID, it.objectID))
			continue;
		Trigger* trigger = new Trigger(screen, it);
		screen->reloadTrigger(trigger);
		screen->addObject(trigger);
	}
}

void MapLoader::loadDynamicTiles(MapData& data, MapScreen* screen) const {
	for (auto& it : data.dynamicTiles) {
		MapDynamicTile* tile = ObjectFactory::Instance()->createMapDynamicTile(it.id, screen);
		if (tile == nullptr) {
			g_logger->logError("MapLoader", "Dynamic tile was not loaded, unknown id.");
			return;
		}

		tile->init();
		tile->setPosition(it.position + tile->getPositionOffset());
		tile->setDebugBoundingBox(COLOR_NEUTRAL);
		tile->loadResources();
		tile->loadAnimation(it.skinNr);

		screen->addObject(tile);

		if (WaypointTile* wpTile = dynamic_cast<WaypointTile*>(tile)) {
			wpTile->setSpawnPosition(it.spawnPosition);
			const CharacterCoreData& coreData = screen->getCharacterCore()->getData();
			if (coreData.waypointsUnlocked.at(data.id).find(it.spawnPosition) != coreData.waypointsUnlocked.at(data.id).end())
				wpTile->setActive();
		}
	}
}