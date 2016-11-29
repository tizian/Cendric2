#include "Map/MapLoader.h"
#include "Map/MapMainCharacter.h"
#include "Map/Map.h"
#include "Map/NPC.h"
#include "LightObject.h"
#include "Screens/MapScreen.h"
#include "ObjectFactory.h"
#include "Map/DynamicTiles/WaypointTile.h"
#include "Map/DynamicTiles/BookTile.h"
#include "Map/DynamicTiles/DoorMapTile.h"
#include "Map/DynamicTiles/SignMapTile.h"
#include "Map/DynamicTiles/ChestMapTile.h"
#include "Trigger.h"

void MapLoader::loadForRenderTexture(MapData& data, MapScreen* screen) {
	loadLights(data, screen);
	loadDynamicTiles(data, screen);
	loadBooks(data, screen);
	loadDoors(data, screen);
	loadSigns(data, screen);
	loadChests(data, screen);
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

void MapLoader::loadBooks(MapData& data, MapScreen* screen) {
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

void MapLoader::loadDoors(MapData& data, MapScreen* screen) {
	// calculate doors
	for (auto& it : data.doors) {
		DoorMapTile* door = new DoorMapTile(screen);
		door->init();
		door->setPosition(it.position + door->getPositionOffset());
		door->setDebugBoundingBox(COLOR_NEUTRAL);
		door->loadResources();
		door->loadAnimation(it.skinNr);
		door->setDoorData(it);

		screen->addObject(door);
	}
}

void MapLoader::loadChests(MapData& data, MapScreen* screen) {
	// calculate doors
	for (auto& it : data.chests) {
		ChestMapTile* chest = new ChestMapTile(screen);
		
		chest->init();
		chest->setDebugBoundingBox(COLOR_NEUTRAL);
		chest->loadResources();
		chest->loadAnimation(it.skinNr);
		chest->setChestData(it);
		chest->setPosition(it.spawnPosition + chest->getPositionOffset());

		screen->addObject(chest);
	}
}

void MapLoader::loadSigns(MapData& data, MapScreen* screen) {
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

		tile->init();
		tile->setPosition(sf::Vector2f(
			(it.spawnPosition % data.mapSize.x) * TILE_SIZE_F,
			(it.spawnPosition / data.mapSize.x) * TILE_SIZE_F) +
			tile->getPositionOffset());
		tile->setDebugBoundingBox(COLOR_NEUTRAL);
		tile->loadResources();
		tile->loadAnimation(it.skinNr);

		screen->addObject(tile);

		if (WaypointTile* wpTile = dynamic_cast<WaypointTile*>(tile)) {
			wpTile->setSpawnPosition(it.spawnPosition);
			const CharacterCoreData& coreData = screen->getCharacterCore()->getData();
			if (contains(coreData.waypointsUnlocked.at(data.id), it.spawnPosition))
				wpTile->setActive();
		}
	}
}