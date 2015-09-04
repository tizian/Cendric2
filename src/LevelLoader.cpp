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
			tile = new CheckpointTile(level);
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

	int x = 0;
	int y = 0;
	const CharacterCoreData& coreData = screen->getCharacterCore()->getData();

	// calculate level positions and create them if they are not looted yet
	for (int i = 0; i < data.levelItems.size(); i++)
	{
		auto& it = data.levelItems.at(i);
		if (!it.empty() && !coreData.itemsLooted.at(data.id).at(i))
		{
			sf::Vector2f position(static_cast<float>(x * data.tileSize.x), static_cast<float>(y * data.tileSize.y));
			const ItemBean* item = g_resourceManager->getItemBean(it);
			if (item == nullptr)
			{
				// unexpected error
				g_logger->logError("LevelLoader", "Level item was not loaded, unknown id.");
				return;
			}

			LevelItem* levelItem = new LevelItem();
			levelItem->load(mainCharacter, *item, position);
			levelItem->setSpawnPosition(i);
			screen->addObject(levelItem);
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
}

void LevelLoader::loadEnemies(LevelData& data, Screen* screen, Level* level) const
{
	LevelMainCharacter* mainCharacter = dynamic_cast<LevelMainCharacter*>(screen->getObjects(GameObjectType::_MainCharacter)->at(0));
	if (mainCharacter == nullptr)
	{
		g_logger->logError("LevelLoader", "Could not find main character of game screen");
		return;
	}

	int x = 0;
	int y = 0;
	const CharacterCoreData& coreData = screen->getCharacterCore()->getData();

	// calculate level positions and create them if they are not looted yet
	for (int i = 0; i < data.enemies.size(); i++)
	{
		auto& it = data.enemies.at(i);
		if (it != EnemyID::VOID && !coreData.enemiesLooted.at(data.id).at(i))
		{
			sf::Vector2f position(static_cast<float>(x * data.tileSize.x), static_cast<float>(y * data.tileSize.y));
			Enemy* enemy = nullptr;
			std::map<string, int> loot;
			switch (it)
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
			case EnemyID::VOID:
			default:
				// unexpected error
				g_logger->logError("LevelLoader", "Enemy was not loaded, unknown id.");
				return;
			}

			enemy->setPosition(position);
			enemy->setSpawnPosition(i);
			if (coreData.enemiesKilled.at(data.id).at(i)) enemy->setDead();
			screen->addObject(enemy);
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
}