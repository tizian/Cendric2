#include "Level/LevelLoader.h"
#include "Level/LevelMainCharacter.h"
#include "Level/Level.h"
#include "Screens/LevelScreen.h"
#include "Level/Enemy.h"
#include "World/LightObject.h"
#include "ObjectFactory.h"
#include "World/Trigger.h"

void LevelLoader::loadAfterMainChar(LevelData& data, LevelScreen* screen, Level* level) const {
	loadEnemies(data, screen, level);
	loadLevelItems(data, screen);
	loadTriggers(data, screen);
}

void LevelLoader::loadDynamicTileData(std::vector<LevelDynamicTileData>& data, LevelScreen* screen) const {
	std::vector<LeverDependentTile*> leverDependentTiles;
	std::vector<LeverTile*> leverTiles;

	for (auto& it : data) {
		LevelDynamicTile* tile = ObjectFactory::Instance()->createLevelDynamicTile(it.id, screen);
		if (tile == nullptr) {
			g_logger->logError("LevelLoader", "Dynamic tile was not loaded, id not registered:" + std::to_string(static_cast<int>(it.id)));
			return;
		}

		tile->setObjectID(it.objectID);
		tile->setPosition(it.position + tile->getPositionOffset());
		tile->loadResources();
		if (!tile->init(it.properties)) {
			g_logger->logError("LevelLoader", "Dynamic tile was not loaded, initialization failed.");
			delete tile;
			continue;
		}

		tile->setPosition(it.position + tile->getPositionOffset());
		tile->loadAnimation(it.skinNr);
		tile->setPosition(it.position + tile->getPositionOffset());
		tile->setDebugBoundingBox(COLOR_NEUTRAL);
		screen->addObject(tile);

		if (LeverDependentTile* dependentTile = dynamic_cast<LeverDependentTile*>(tile)) {
			leverDependentTiles.push_back(dependentTile);
		}
		else if (LeverTile* leverTile = dynamic_cast<LeverTile*>(tile)) {
			leverTiles.push_back(leverTile);
		}
	}

	// set levers
	for (auto leverTile : leverTiles) {
		leverTile->setDependentTiles(leverDependentTiles);
	}
}

void LevelLoader::loadDynamicTiles(LevelData& data, LevelScreen* screen) const {
	loadDynamicTileData(data.dynamicTiles, screen);
	for (auto& it : data.levers) {
		loadDynamicTileData(it, screen);
	}
}

void LevelLoader::loadLevelItems(LevelData& data, LevelScreen* screen) const {
	int x = 0;
	int y = 0;
	const CharacterCoreData& coreData = screen->getCharacterCore()->getData();

	// calculate level positions and create them if they are not looted yet
	for (size_t i = 0; i < data.levelItems.size(); ++i) {
		auto& it = data.levelItems.at(i);
		if (!it.empty() && !contains(coreData.itemsLooted.at(data.id), static_cast<int>(i))) {
			sf::Vector2f position(x * TILE_SIZE_F, y * TILE_SIZE_F);
			if (!g_resourceManager->getItem(it)) {
				// unexpected error
				g_logger->logError("LevelLoader", "Level item was not loaded, unknown id: " + it);
				return;
			}

			LevelItem* levelItem = new LevelItem(screen);
			levelItem->load(it, position);
			levelItem->setSpawnPosition(static_cast<int>(i));
			screen->addObject(levelItem);
		}
		if (x + 1 >= data.mapSize.x) {
			x = 0;
			y++;
		}
		else {
			x++;
		}
	}
}

void LevelLoader::loadTriggers(LevelData& data, LevelScreen* screen) const {
	for (auto& it : data.triggers) {
		if (screen->getCharacterCore()->isTriggerTriggered(it.worldID, it.objectID))
			continue;
		Trigger* trigger = new Trigger(screen, it);
		screen->reloadTrigger(trigger);
		screen->addObject(trigger);
	}
}

void LevelLoader::loadEnemies(LevelData& data, LevelScreen* screen, Level* level) const {
	const CharacterCore* core = screen->getCharacterCore();

	for (auto& it : data.enemies) {
		// an unique enemy that is looted will not respawn!
		if (it.isUnique && core->isEnemyLooted(data.id, it.objectID)) continue;

		// create enemy
		Enemy* enemy = nullptr;
		enemy = ObjectFactory::Instance()->createEnemy(it.id, level, screen);
		if (enemy == nullptr) {
			g_logger->logError("LevelLoader", "Enemy was not loaded, unknown id.");
			return;
		}

		enemy->setEnemyName(it.name);
		enemy->load(it.skinNr);

		// set quest targets
		for (auto& target : it.questTargets) {
			if (!target.first.empty()) {
				enemy->addQuestTarget(target);
			}
		}

		// set quest condition
		if (!it.questCondition.first.empty()) {
			enemy->addQuestCondition(it.questCondition);
		}

		// calculate loot.
		std::map<std::string, int> loot;
		int gold = 0;

		if (core->isEnemyLooted(data.id, it.objectID)) {
			enemy->insertRespawnLoot(loot, gold);
		}
		else {
			loot = it.customizedLoot.first;
			gold = it.customizedLoot.second;
			if (loot.empty() && gold == 0) {
				enemy->insertDefaultLoot(loot, gold);
			}
		}

		enemy->setLoot(loot, gold);
		enemy->setPosition(it.spawnPosition);
		enemy->setObjectID(it.objectID);
		enemy->setUnique(it.isUnique);
		enemy->setDebugBoundingBox(sf::Color::Magenta);
		if ((it.isUnique && core->isEnemyKilled(data.id, it.objectID)) || it.isDead) enemy->setDead();
		screen->addObject(enemy);
		if (!it.luaPath.empty()) {
			enemy->setScriptedBehavior(it.luaPath);
		}
	}
}

void LevelLoader::loadLights(LevelData& data, LevelScreen* screen) const {
	// calculate lights
	for (auto& it : data.lights) {
		LightObject* lightObject = new LightObject(it);
		screen->addObject(lightObject);
	}
}