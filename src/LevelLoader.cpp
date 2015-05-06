#include "LevelLoader.h"
#include "MainCharacter.h"

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

void LevelLoader::loadLevelItems(LevelData& data, Screen* screen)const
{
	MainCharacter* mainCharacter = dynamic_cast<MainCharacter*>(screen->getObjects(GameObjectType::_MainCharacter)->at(0));
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
			item.spriteOffset = sf::Vector2f(-10.f, -10.f);
			item.boundingBox = sf::FloatRect(0, 0, 30, 30);
			item.textureID = ResourceID::Texture_items_food;
			item.texturePos = sf::IntRect(0, 0, 50, 50);
			item.frameTime = sf::seconds(1.0f);
			item.tooltip = Texts::Levelitem_tooltip_cheese;
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
		idleAnimation.setSpriteSheet(g_resourceManager->getTexture(item.textureID));
		idleAnimation.addFrame(item.texturePos);
		levelItem->addAnimation(GameObjectState::Idle, idleAnimation);
		levelItem->setFrameTime(item.frameTime);
		// initial values
		levelItem->setCurrentAnimation(levelItem->getAnimation(GameObjectState::Idle), false);
		levelItem->playCurrentAnimation(false);
		levelItem->loadItem(mainCharacter, it->first);
		levelItem->setTooltipText(g_textProvider->getText(item.tooltip));
		levelItem->setPosition(it->second - levelItem->getSpriteOffset());
		screen->addObject(GameObjectType::_LevelItem, levelItem);
	}
}

void LevelLoader::loadEnemies(LevelData& data, Screen* screen) const
{
	// TODO
}