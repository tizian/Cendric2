#include "Level/LevelLoader.h"
#include "Level/LevelMainCharacter.h"
#include "Level/Level.h"
#include "Screens/LevelScreen.h"
#include "Level/Enemy.h"
#include "LightObject.h"
#include "ObjectFactory.h"
#include "Level/DynamicTiles/ModifierTile.h"
#include "Level/DynamicTiles/MovingTile.h"

using namespace std;

void LevelLoader::loadAfterMainChar(LevelData& data, Screen* screen, Level* level) const {
	loadEnemies(data, screen, level);
	loadLevelItems(data, screen);
	loadModifierTiles(data, screen, level);
	loadChestTiles(data, screen, level);
	loadLeverTiles(data, screen, level);
	loadMovingTiles(data, screen, level);
}

void LevelLoader::loadChestTiles(LevelData& data, Screen* screen, Level* level) const {
	LevelMainCharacter* mainCharacter = dynamic_cast<LevelScreen*>(screen)->getMainCharacter();
	if (mainCharacter == nullptr) {
		g_logger->logError("LevelLoader", "Could not find main character of game screen");
		return;
	}

	const CharacterCoreData& coreData = screen->getCharacterCore()->getData();

	// create chests if they are not looted yet
	for (auto& it : data.chests) {
		if (coreData.chestsLooted.at(data.id).find(it.objectID) != coreData.chestsLooted.at(data.id).end()) {
			continue;
		}

		ChestTile* chestTile = nullptr;

		// calculate loot.
		std::map<string, int> loot = it.loot.first;
		int gold = it.loot.second;

		chestTile = new ChestTile(mainCharacter, level);
		chestTile->init();
		chestTile->setObjectID(it.objectID);
		chestTile->setStrength(it.chestStrength);
		chestTile->setLoot(loot, gold);
		chestTile->setPosition(it.spawnPosition - chestTile->getPositionOffset());
		chestTile->setDebugBoundingBox(sf::Color::Yellow);
		chestTile->loadAnimation(it.skinNr);
		screen->addObject(chestTile);
	}
}

void LevelLoader::loadMovingTiles(LevelData& data, Screen* screen, Level* level) const {

	for (auto& movingData : data.movingTiles) {

		MovingTile* movingTile = new MovingTile(level);

		movingTile->setMovingTileData(movingData);
		movingTile->init();
		movingTile->setDebugBoundingBox(sf::Color::Yellow);
		movingTile->loadAnimation(movingData.skinNr);
		movingTile->setPosition(movingData.spawnPosition);
		movingTile->setDynamicTileID(LevelDynamicTileID::Moving);

		screen->addObject(movingTile);
	}
}

void LevelLoader::loadModifierTiles(LevelData& data, Screen* screen, Level* level) const {
	const CharacterCoreData& coreData = screen->getCharacterCore()->getData();

	// create modifier tiles if they are not learned yet
	for (auto& modifierData : data.modifiers) {
		if (coreData.modfiersLearned.find(modifierData.modifier.type) != coreData.modfiersLearned.end()
			&& coreData.modfiersLearned.at(modifierData.modifier.type) >= modifierData.modifier.level) {
			continue;
		}

		ModifierTile* modifierTile = new ModifierTile(level);

		modifierTile->setModifier(modifierData.modifier);
		modifierTile->init();
		modifierTile->loadAnimation(0);
		modifierTile->setDebugBoundingBox(sf::Color::Yellow);
		modifierTile->setPosition(modifierData.spawnPosition);

		screen->addObject(modifierTile);
	}
}


void LevelLoader::loadLeverTiles(LevelData& data, Screen* screen, Level* level) const {
	LevelMainCharacter* mainCharacter = dynamic_cast<LevelScreen*>(screen)->getMainCharacter();
	if (mainCharacter == nullptr) {
		g_logger->logError("LevelLoader", "Could not find main character of game screen");
		return;
	}

	for (auto& it : data.levers) {

		std::vector<SwitchableTile*> dependentTiles;

		// create the switch tiles and add them.
		for (auto& switchBean : it.dependentTiles) {
			SwitchableTile* tile = new SwitchableTile(level);
			tile->setInitialState(switchBean.id == LevelDynamicTileID::SwitchableOn);
			tile->init();
			tile->setPosition(switchBean.position);
			tile->setDebugBoundingBox(sf::Color::Yellow);
			tile->loadAnimation(switchBean.skinNr);
			screen->addObject(tile);
			dependentTiles.push_back(tile);
		}

		// create the lever tiles and add them.
		for (auto& leverBean : it.levers) {
			LeverTile* tile = new LeverTile(level, mainCharacter);
			tile->init();
			tile->setPosition(leverBean.position);
			tile->setDebugBoundingBox(sf::Color::Yellow);
			tile->loadAnimation(leverBean.skinNr);
			tile->setDependantTiles(dependentTiles);
			screen->addObject(tile);
		}
	}
}

void LevelLoader::loadDynamicTiles(LevelData& data, Screen* screen, Level* level) const {
	LevelMainCharacter* mainCharacter = dynamic_cast<LevelScreen*>(screen)->getMainCharacter();
	if (mainCharacter == nullptr) {
		g_logger->logError("LevelLoader", "Could not find main character of game screen");
		return;
	}

	for (auto& it : data.dynamicTiles) {
		LevelDynamicTile* tile = ObjectFactory::Instance()->createLevelDynamicTile(it.id, level);
		if (tile == nullptr) {
			g_logger->logError("LevelLoader", "Dynamic tile was not loaded, unknown id.");
			return;
		}

		// special behavior
		switch (it.id) {
		case LevelDynamicTileID::Fluid:
			tile->setBoundingBox(sf::FloatRect(0.f, 0.f, it.size.x, it.size.y));
			break;
		default:
			break;
		}

		tile->init();
		tile->setPosition(it.position - tile->getPositionOffset());
		tile->setDebugBoundingBox(sf::Color::Yellow);
		tile->loadAnimation(it.skinNr);
		tile->setDynamicTileID(it.id);
		screen->addObject(tile);
	}
}

void LevelLoader::loadLevelItems(LevelData& data, Screen* screen) const {
	LevelMainCharacter* mainCharacter = dynamic_cast<LevelScreen*>(screen)->getMainCharacter();
	if (mainCharacter == nullptr) {
		g_logger->logError("LevelLoader", "Could not find main character of game screen");
		return;
	}

	int x = 0;
	int y = 0;
	const CharacterCoreData& coreData = screen->getCharacterCore()->getData();

	// calculate level positions and create them if they are not looted yet
	for (int i = 0; i < data.levelItems.size(); i++) {
		auto& it = data.levelItems.at(i);
		if (!it.empty() && (coreData.itemsLooted.at(data.id).find(i) == coreData.itemsLooted.at(data.id).end())) {
			sf::Vector2f position(x * TILE_SIZE_F, y * TILE_SIZE_F);
			ItemBean item = g_databaseManager->getItemBean(it);
			if (item.status == BeanStatus::NotSet) {
				// unexpected error
				g_logger->logError("LevelLoader", "Level item was not loaded, unknown id.");
				return;
			}

			LevelItem* levelItem = new LevelItem();
			levelItem->load(mainCharacter, Item(item.item_id), position);
			levelItem->setSpawnPosition(i);
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

void LevelLoader::loadEnemies(LevelData& data, Screen* screen, Level* level) const {
	LevelMainCharacter* mainCharacter = dynamic_cast<LevelScreen*>(screen)->getMainCharacter();
	if (mainCharacter == nullptr) {
		g_logger->logError("LevelLoader", "Could not find main character of game screen");
		return;
	}

	const CharacterCoreData& coreData = screen->getCharacterCore()->getData();

	// create enemies if they are not looted yet
	for (auto& it : data.enemies) {
		if (coreData.enemiesLooted.at(data.id).find(it.objectID) == coreData.enemiesLooted.at(data.id).end()) {
			Enemy* enemy = nullptr;
			// calculate loot.
			std::map<string, int> loot = it.customizedLoot.first;
			int gold = it.customizedLoot.second;

			enemy = ObjectFactory::Instance()->createEnemy(it.id, level, screen);
			if (enemy == nullptr) {
				g_logger->logError("LevelLoader", "Enemy was not loaded, unknown id.");
				return;
			}
			if (!it.questTarget.first.empty()) {
				enemy->setQuestTarget(it.questTarget);
			}

			enemy->insertDefaultLoot(loot, gold);
			enemy->setLoot(loot, gold);
			enemy->setPosition(it.spawnPosition);
			enemy->setObjectID(it.objectID);
			enemy->setPersistent(it.isPersistent);
			enemy->setDebugBoundingBox(sf::Color::Magenta);
			if (coreData.enemiesKilled.at(data.id).find(it.objectID) != coreData.enemiesKilled.at(data.id).end()) enemy->setDead();
			screen->addObject(enemy);
			if (!it.luaPath.empty()) {
				enemy->setScriptedBehavior(it.luaPath);
			}
		}
	}
}

void LevelLoader::loadLights(LevelData& data, Screen* screen) const {
	// calculate lights
	for (auto& it : data.lights) {
		LightObject* lightObject = new LightObject(it);
		screen->addObject(lightObject);
	}
}