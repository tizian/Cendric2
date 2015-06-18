#include "LevelLoader.h"
#include "LevelMainCharacter.h"
#include "Level.h"
#include "Enemy.h"
#include "Enemies/RatEnemy.h"
#include "Enemies/FireRatEnemy.h"

void LevelLoader::loadDynamicTiles(LevelData& data, Screen* screen) const
{
	for (std::vector<std::pair<DynamicTileID, sf::Vector2f>>::iterator it = data.dynamicTilePositions.begin(); it != data.dynamicTilePositions.end(); ++it)
	{
		DynamicTile* tile;
		switch (it->first)
		{
		case DynamicTileID::Water:
			tile = new WaterTile();
			break;
		case DynamicTileID::Ice:
			tile = new IceTile();
			break;
		case DynamicTileID::Crumbly_block:
			tile = new CrumblyBlockTile();
			break;
		case DynamicTileID::Torch:
			tile = new TorchTile();
			break;
		case DynamicTileID::Chest:
			tile = new ChestTile();
			break;
		default:
			// unexpected error
			g_logger->logError("LevelLoader", "Dynamic tile was not loaded, unknown id.");
			return;
		}

		tile->setTileSize(data.tileSize);
		tile->load();
		tile->setPosition(it->second);
		screen->addObject(GameObjectType::_DynamicTile, tile);
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

	for (std::vector<std::pair<LevelItemID, sf::Vector2f>>::iterator it = data.levelItemPositions.begin(); it != data.levelItemPositions.end(); ++it)
	{
		LevelItemBean item;
		switch (it->first)
		{
		case LevelItemID::Food_Cheese:
			item.spriteOffset = sf::Vector2f(-14.f, -14.f);
			item.boundingBox = sf::FloatRect(0, 0, 22, 22);
			item.texturePositions.push_back(sf::IntRect(0, 0, 50, 50));
			item.frameTime = sf::seconds(1.0f);
			item.tooltip = "LevelItemTooltipCheese";
			item.item = ItemID::Food_Cheese;
			break;
		case LevelItemID::EQ_Ring_RingOfLesserHealth:
			item.spriteOffset = sf::Vector2f(-14.f, -40.f);
			item.boundingBox = sf::FloatRect(0, 0, 5, 5);
			item.texturePositions.push_back(sf::IntRect(200, 0, 50, 50));
			item.frameTime = sf::seconds(1.0f);
			item.tooltip = "LevelItemTooltipRingOfLesserHealth";
			item.item = ItemID::Equipment_ring_ringOfLesserHealth;
			break;
		case LevelItemID::Food_Bread:
			item.spriteOffset = sf::Vector2f(-10.f, -25.f);
			item.boundingBox = sf::FloatRect(0, 0, 30, 5);
			item.texturePositions.push_back(sf::IntRect(50, 100, 50, 50));
			item.frameTime = sf::seconds(1.0f);
			item.tooltip = "LevelItemTooltipBread";
			item.item = ItemID::Food_Bread;
			break;
		case LevelItemID::Food_CaveBerry:
			item.spriteOffset = sf::Vector2f(0.f, -30.f);
			item.boundingBox = sf::FloatRect(0, 0, 50, 70);
			item.texturePositions.push_back(sf::IntRect(0, 0, 50, 100));
			item.frameTime = sf::seconds(1.0f);
			item.tooltip = "LevelItemTooltipCaveBerry";
			item.item = ItemID::Food_CaveBerry;
			break;
		case LevelItemID::Food_GlowingShroom:
			item.spriteOffset = sf::Vector2f(-10.f, -10.f);
			item.boundingBox = sf::FloatRect(0, 0, 30, 30);
			item.texturePositions.push_back(sf::IntRect(100, 50, 50, 50));
			item.frameTime = sf::seconds(1.0f);
			item.tooltip = "LevelItemTooltipGlowingShroom";
			item.item = ItemID::Food_GlowingShroom;
			break;
		case LevelItemID::Food_HealingHerb:
			item.spriteOffset = sf::Vector2f(-10.f, 0.f);
			item.boundingBox = sf::FloatRect(0, 0, 30, 50);
			item.texturePositions.push_back(sf::IntRect(50, 50, 50, 50));
			item.frameTime = sf::seconds(1.0f);
			item.tooltip = "LevelItemTooltipHealingHerb";
			item.item = ItemID::Food_HealingHerb;
			break;
		case LevelItemID::Food_Water:
			item.spriteOffset = sf::Vector2f(-16.f, -10.f);
			item.boundingBox = sf::FloatRect(0, 0, 18, 30);
			item.texturePositions.push_back(sf::IntRect(200, 50, 50, 50));
			item.frameTime = sf::seconds(1.0f);
			item.tooltip = "LevelItemTooltipWater";
			item.item = ItemID::Food_Water;
			break;
		case LevelItemID::Gold_OneCoin:
			item.spriteOffset = sf::Vector2f(-20.f, -40.f);
			item.boundingBox = sf::FloatRect(0, 0, 5, 5);
			item.texturePositions.push_back(sf::IntRect(100, 0, 50, 50));
			item.frameTime = sf::seconds(1.0f);
			item.tooltip = "LevelItemTooltipOneCoin";
			item.item = ItemID::Void;
			item.goldValue = 1;
			break;
		case LevelItemID::Gold_ThreeCoins:
			item.spriteOffset = sf::Vector2f(-5.f, -25.f);
			item.boundingBox = sf::FloatRect(0, 0, 25, 25);
			item.texturePositions.push_back(sf::IntRect(150, 50, 50, 50));
			item.frameTime = sf::seconds(1.0f);
			item.tooltip = "LevelItemTooltipThreeCoins";
			item.item = ItemID::Void;
			item.goldValue = 3;
			break;
		case LevelItemID::Misc_GoldenGoblet:
			item.spriteOffset = sf::Vector2f(-10.f, -5.f);
			item.boundingBox = sf::FloatRect(0, 0, 30, 40);
			item.texturePositions.push_back(sf::IntRect(50, 0, 50, 50));
			item.frameTime = sf::seconds(1.0f);
			item.tooltip = "LevelItemTooltipGoldenGoblet";
			item.item = ItemID::Misc_GoldenGoblet;
			break;
		case LevelItemID::Quest_Letter:
			item.spriteOffset = sf::Vector2f(0.f, -20.f);
			item.boundingBox = sf::FloatRect(0, 0, 50, 30);
			item.texturePositions.push_back(sf::IntRect(150, 0, 50, 50));
			item.frameTime = sf::seconds(1.0f);
			item.tooltip = "LevelItemTooltipLetter";
			item.item = ItemID::Quest_Letter;
			break;
		default:
			// unexpected error
			g_logger->logError("LevelLoader", "Level item was not loaded, unknown id.");
			return;
		}

		LevelItem* levelItem = new LevelItem();
		Animation idleAnimation;
		levelItem->setSpriteOffset(item.spriteOffset);
		levelItem->setBoundingBox(item.boundingBox);
		idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_levelitems));
		// add frames
		for (auto &frame : item.texturePositions) {
			idleAnimation.addFrame(frame);
		}
		levelItem->addAnimation(GameObjectState::Idle, idleAnimation);
		levelItem->setFrameTime(item.frameTime);
		// initial values
		levelItem->setCurrentAnimation(levelItem->getAnimation(GameObjectState::Idle), false);
		levelItem->playCurrentAnimation(item.texturePositions.size() > 1);
		levelItem->loadItem(mainCharacter, it->first);
		levelItem->setTooltipText(g_textProvider->getText(item.tooltip));
		levelItem->setItemID(item.item);
		levelItem->setPosition(it->second - levelItem->getSpriteOffset());
		screen->addObject(GameObjectType::_LevelItem, levelItem);
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
		std::map<ItemID, int> loot;
		switch (it.first)
		{
		case EnemyID::Rat:
			enemy = new RatEnemy(level, mainCharacter);
			loot.insert({ ItemID::Food_Cheese, 1 });
			enemy->setLoot(loot, 1);
			break;
		case EnemyID::FireRat:
			enemy = new FireRatEnemy(level, mainCharacter);
			loot.insert({ ItemID::Food_Bread, 2 });
			enemy->setLoot(loot, 2);
			break;
		case EnemyID::Void:
		default:
			// unexpected error
			g_logger->logError("LevelLoader", "Enemy was not loaded, unknown id.");
			return;
		}

		enemy->setPosition(it.second);
		screen->addObject(GameObjectType::_Enemy, enemy);
	}
}