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
		LevelItem* levelItem;
		switch (it->first)
		{
		case LevelItemID::Food_Cheese:
			levelItem = new CheeseLevelItem();
			break;
		default:
			// unexpected error
			g_logger->logError("LevelLoader", "Level item was not loaded, unknown id.");
			return;
		}

		levelItem->loadItem(mainCharacter);
		levelItem->setPosition(it->second - levelItem->getSpriteOffset());
		screen->addObject(GameObjectType::_LevelItem, levelItem);
	}
}

void LevelLoader::loadEnemies(LevelData& data, Screen* screen) const
{
	// TODO
}