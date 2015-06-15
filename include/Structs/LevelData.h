#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "BackgroundLayer.h"
#include "Structs/LevelExitBean.h"

struct LevelData
{
	std::string name;
	sf::Vector2i mapSize;
	sf::Vector2i tileSize;
	sf::Vector2f startPos;
	std::string tileSetPath;
	std::vector<BackgroundLayer> backgroundLayers;
	std::vector<std::vector<int>> layers;
	std::vector<bool> collidableTiles;
	std::vector<std::vector<bool>> collidableTilePositions;
	std::vector<std::pair<DynamicTileID, std::vector<bool>>> dynamicTiles;
	std::vector<std::pair<DynamicTileID, sf::Vector2f>> dynamicTilePositions;
	std::vector<LevelItemID> levelItems;
	std::vector<std::pair<LevelItemID, sf::Vector2f>> levelItemPositions;
	std::vector<EnemyID> enemies;
	std::vector<std::pair<EnemyID, sf::Vector2f>> enemyPositions;
	std::vector<LevelExitBean> levelExits;
	sf::FloatRect levelRect;
};