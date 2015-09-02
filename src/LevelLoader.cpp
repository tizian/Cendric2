#include "LevelLoader.h"
#include "LevelMainCharacter.h"
#include "Level.h"
#include "Enemy.h"
#include "Enemies/RatEnemy.h"
#include "Enemies/FireRatEnemy.h"

using namespace std;

void LevelLoader::loadDynamicTiles(LevelData& data, Screen* screen, Level* level) const
{
	for (auto& it : data.dynamicTiles)
	{
		DynamicTile* tile;
		switch (it.id)
		{
		case DynamicTileID::Water:
			tile = new SimulatedWaterTile(level);
			tile->setBoundingBox(sf::FloatRect(0.f, 0.f, it.size.x, it.size.y));
			break;
		case DynamicTileID::Ice:
			tile = new IceTile(level);
			break;
		case DynamicTileID::CrumblyBlock:
			tile = new CrumblyBlockTile(level);
			break;
		case DynamicTileID::Torch:
			tile = new TorchTile(level);
			break;
		case DynamicTileID::Chest:
			tile = new ChestTile(level); 
			break;
		case DynamicTileID::SpikesBottom:
			tile = new SpikesBottomTile(level);
			break;
		case DynamicTileID::SpikesTop:
			tile = new SpikesTopTile(level);
			break;
		case DynamicTileID::ShiftableBlock:
			tile = new ShiftableBlockTile(level);
			break;
		case DynamicTileID::Checkpoint:
			tile = new CheckpointTile(screen->getCharacterCore(), level);
			break;
		default:
			// unexpected error
			g_logger->logError("LevelLoader", "Dynamic tile was not loaded, unknown id.");
			return;
		}

		tile->setTileSize(data.tileSize);
		tile->init();
		tile->setPosition(it.position - tile->getPositionOffset());
		tile->setDebugBoundingBox(sf::Color::Yellow);
		tile->load(it.skinNr);
		screen->addObject(tile);
	}
}

void LevelLoader::loadLevelItems(LevelData& data, Screen* screen) const
{
	LevelMainCharacter* mainCharacter = dynamic_cast<LevelMainCharacter*>(screen->getObjects(GameObjectType::_MainCharacter)->at(0));
	if (mainCharacter == nullptr)
	{	
		g_logger->logError("LevelLoader", "Could not find main character of game screen");
		return;
	}

	for (std::vector<std::pair<string, sf::Vector2f>>::iterator it = data.levelItemPositions.begin(); it != data.levelItemPositions.end(); ++it)
	{
		const ItemBean* item = g_resourceManager->getItemBean(it->first);
		if (item == nullptr)
		{
			// unexpected error
			g_logger->logError("LevelLoader", "Level item was not loaded, unknown id.");
			return;
		}

		LevelItem* levelItem = new LevelItem();
		levelItem->load(mainCharacter, *item, it->second);
		screen->addObject(levelItem);
	}
}

void LevelLoader::loadEnemies(LevelData& data, Screen* screen, Level* level) const
{
	LevelMainCharacter* mainCharacter = dynamic_cast<LevelMainCharacter*>(screen->getObjects(GameObjectType::_MainCharacter)->at(0));
	if (mainCharacter == nullptr)
	{
		g_logger->logError("LevelLoader", "Could not find main character of game screen");
		return;
	}

	for (auto it : data.enemyPositions)
	{
		Enemy* enemy = nullptr;
		std::map<string, int> loot;
		switch (it.first)
		{
		case EnemyID::Rat:
			enemy = new RatEnemy(level, mainCharacter);
			loot.insert({ "it_fo_cheese", 1 });
			enemy->setLoot(loot, 1);
			break;
		case EnemyID::FireRat:
			enemy = new FireRatEnemy(level, mainCharacter);
			loot.insert({ "it_fo_bread", 2 });
			enemy->setLoot(loot, 2);
			break;
		case EnemyID::Void:
		default:
			// unexpected error
			g_logger->logError("LevelLoader", "Enemy was not loaded, unknown id.");
			return;
		}

		enemy->setPosition(it.second);
		enemy->setDebugBoundingBox(sf::Color::Magenta);
		screen->addObject(enemy);
	}
}