#include "Level/LevelLoader.h"
#include "Level/LevelMainCharacter.h"
#include "Level/Level.h"
#include "Screens/LevelScreen.h"
#include "Level/Enemy.h"
#include "LightObject.h"
#include "ObjectFactory.h"
#include "Level/DynamicTiles/ModifierTile.h"
#include "Level/DynamicTiles/MovingTile.h"
#include "Level/DynamicTiles/JumpingTile.h"
#include "Level/DynamicTiles/FallingTile.h"
#include "Level/DynamicTiles/SignLevelTile.h"
#include "Level/DynamicTiles/LadderTile.h"
#include "Level/DynamicTiles/ResourceTile.h"
#include "Level/DynamicTiles/DoorLevelTile.h"
#include "Trigger.h"

using namespace std;

void LevelLoader::loadAfterMainChar(LevelData& data, LevelScreen* screen, Level* level) const {
	loadEnemies(data, screen, level);
	loadLevelItems(data, screen);
	loadTriggers(data, screen);
}

void LevelLoader::loadChestTiles(LevelData& data, LevelScreen* screen) const {
	LevelMainCharacter* mainCharacter = screen->getMainCharacter();
	if (mainCharacter == nullptr) {
		g_logger->logError("LevelLoader", "Could not find main character of game screen");
		return;
	}

	const CharacterCoreData& coreData = screen->getCharacterCore()->getData();

	// create chests if they are not looted yet
	for (auto& it : data.chests) {
		if (contains(coreData.chestsLooted.at(data.id), it.objectID)) {
			continue;
		}

		ChestLevelTile* chestTile = new ChestLevelTile(screen);
		chestTile->init();	
		chestTile->setDebugBoundingBox(COLOR_NEUTRAL);
		chestTile->loadResources();
		chestTile->loadAnimation(it.skinNr);
		chestTile->setChestData(it);
		chestTile->setPosition(it.spawnPosition + chestTile->getPositionOffset());
		screen->addObject(chestTile);
	}
}

void LevelLoader::loadJumpingTiles(LevelData& data, LevelScreen* screen) const {
	for (auto& jumpingData : data.jumpingTiles) {

		JumpingTile* jumpingTile = new JumpingTile(screen);

		jumpingTile->setJumpingTileData(jumpingData);
		jumpingTile->init();
		jumpingTile->setDebugBoundingBox(COLOR_NEUTRAL);
		jumpingTile->loadResources();
		jumpingTile->loadAnimation(jumpingData.skinNr);
		jumpingTile->setPosition(jumpingData.spawnPosition);

		screen->addObject(jumpingTile);
	}
}

void LevelLoader::loadDoorTiles(LevelData& data, LevelScreen* screen) const {
	for (auto& doorData : data.doors) {

		DoorLevelTile* doorTile = new DoorLevelTile(screen);

		doorTile->setDoorData(doorData);
		doorTile->init();
		doorTile->setDebugBoundingBox(COLOR_NEUTRAL);
		doorTile->loadResources();
		doorTile->loadAnimation(doorData.skinNr);
		doorTile->setPosition(doorData.position);

		screen->addObject(doorTile);
	}
}

void LevelLoader::loadSignTiles(LevelData& data, LevelScreen* screen) const {
	for (auto& signData : data.signs) {

		SignLevelTile* signTile = new SignLevelTile(signData, screen);

		signTile->init();
		signTile->setPosition(signData.position + signTile->getPositionOffset());
		signTile->setDebugBoundingBox(COLOR_NEUTRAL);
		signTile->loadResources();
		signTile->loadAnimation(signData.skinNr);

		screen->addObject(signTile);
	}
}

void LevelLoader::loadLadderTiles(LevelData& data, LevelScreen* screen) const {
	for (auto& ladderData : data.ladderTiles) {

		LadderTile* ladderTile = new LadderTile(ladderData, screen);

		ladderTile->init();
		ladderTile->setDebugBoundingBox(COLOR_NEUTRAL);
		ladderTile->loadResources();
		ladderTile->loadAnimation(ladderData.skinNr);
		ladderTile->setPosition(ladderData.position + ladderTile->getPositionOffset());

		screen->addObject(ladderTile);
	}
}

void LevelLoader::loadModifierTiles(LevelData& data, LevelScreen* screen) const {
	const CharacterCoreData& coreData = screen->getCharacterCore()->getData();

	// create modifier tiles if they are not learned yet
	for (auto& modifierData : data.modifiers) {
		if (contains(coreData.modfiersLearned, modifierData.modifier.type)
			&& coreData.modfiersLearned.at(modifierData.modifier.type) >= modifierData.modifier.level) {
			continue;
		}

		ModifierTile* modifierTile = new ModifierTile(screen);

		modifierTile->setModifier(modifierData.modifier);
		modifierTile->init();
		modifierTile->loadResources();
		modifierTile->loadAnimation(0);
		modifierTile->setDebugBoundingBox(COLOR_NEUTRAL);
		modifierTile->setPosition(modifierData.spawnPosition);

		screen->addObject(modifierTile);
	}
}


void LevelLoader::loadLeverTiles(LevelData& data, LevelScreen* screen) const {
	for (auto& it : data.levers) {

		std::vector<LeverDependentTile*> dependentTiles;

		// create the switch tiles and add them.
		for (auto& switchBean : it.dependentSwitchableTiles) {
			SwitchableTile* tile = new SwitchableTile(screen);

			tile->setInitialState(switchBean.id == LevelDynamicTileID::SwitchableOn);
			tile->init();
			tile->setPosition(switchBean.position);
			tile->setDebugBoundingBox(COLOR_NEUTRAL);
			tile->loadResources();
			tile->loadAnimation(switchBean.skinNr);
			screen->addObject(tile);
			dependentTiles.push_back(tile);
		}

		// create the moving tiles and add them.
		for (auto& movingData : it.dependentMovingTiles) {
			MovingTile* movingTile = new MovingTile(screen);

			movingTile->setMovingTileData(movingData);
			movingTile->init();
			movingTile->setDebugBoundingBox(COLOR_NEUTRAL);
			movingTile->loadResources();
			movingTile->loadAnimation(movingData.skinNr);
			movingTile->setPosition(movingData.spawnPosition);
			screen->addObject(movingTile);
			dependentTiles.push_back(movingTile);
		}

		// create the lever tiles and add them.
		for (auto& leverBean : it.levers) {
			LeverTile* tile = new LeverTile(screen);

			tile->init();
			tile->setPosition(leverBean.position);
			tile->setDebugBoundingBox(COLOR_NEUTRAL);
			tile->loadResources();
			tile->loadAnimation(leverBean.skinNr);
			tile->setDependentTiles(dependentTiles);
			screen->addObject(tile);
		}
	}
}

void LevelLoader::loadDynamicTiles(LevelData& data, LevelScreen* screen) const {
	for (auto& it : data.dynamicTiles) {
		LevelDynamicTile* tile = ObjectFactory::Instance()->createLevelDynamicTile(it.id, screen);
		if (tile == nullptr) {
			g_logger->logError("LevelLoader", "Dynamic tile was not loaded, unknown id.");
			return;
		}

		if (it.id != LevelDynamicTileID::Fluid) {
			it.position = sf::Vector2f(
				(it.spawnPosition % data.mapSize.x) * TILE_SIZE_F,
				(it.spawnPosition / data.mapSize.x) * TILE_SIZE_F);
		}

		// special behavior
		switch (it.id) {
		case LevelDynamicTileID::Fluid:
			tile->setBoundingBox(sf::FloatRect(0.f, 0.f, it.size.x, it.size.y));
			break;
		case LevelDynamicTileID::Falling:
			(dynamic_cast<FallingTile*>(tile))->setInitialHeight(it.position.y + tile->getPositionOffset().y);
			break;
		default:
			break;
		}

		tile->init();
		tile->setPosition(it.position + tile->getPositionOffset());
		tile->loadResources();
		tile->loadAnimation(it.skinNr);
		tile->setPosition(it.position + tile->getPositionOffset());
		tile->setDebugBoundingBox(COLOR_NEUTRAL);
		screen->addObject(tile);
	}

	loadModifierTiles(data, screen);
	loadChestTiles(data, screen);
	loadLeverTiles(data, screen);
	loadJumpingTiles(data, screen);
	loadSignTiles(data, screen);
	loadLadderTiles(data, screen);
	loadDoorTiles(data, screen);
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
			ItemBean item = g_databaseManager->getItemBean(it);
			if (item.status == BeanStatus::NotSet) {
				// unexpected error
				g_logger->logError("LevelLoader", "Level item was not loaded, unknown id: " + it);
				return;
			}

			LevelItem* levelItem = new LevelItem(screen);
			levelItem->load(item.item_id, position);
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
		std::map<string, int> loot;
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
		if (it.isUnique && core->isEnemyKilled(data.id, it.objectID)) enemy->setDead();
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