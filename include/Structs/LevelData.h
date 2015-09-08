#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "BackgroundLayer.h"
#include "Structs/LevelExitBean.h"
#include "Structs/DynamicTileBean.h"

struct LevelData
{
	std::string id;
	std::string name;
	sf::Vector2i mapSize;
	sf::Vector2i tileSize;
	std::string tileSetPath;
	std::vector<BackgroundLayer> backgroundLayers;
	std::vector<std::vector<int>> backgroundTileLayers;
	std::vector<std::vector<int>> foregroundTileLayers;
	std::vector<bool> collidableTiles;
	std::vector<std::vector<bool>> collidableTilePositions;
	std::vector<std::pair<DynamicTileID, std::vector<int>>> dynamicTileLayers;
	std::vector<DynamicTileBean> dynamicTiles;
	std::vector<std::string> levelItems;
	std::map<int, std::pair<EnemyID, sf::Vector2f>> enemies;
	std::map<int, std::pair<int, sf::Vector2f>> chests;
	std::map<int, int> chestStrength;
	std::map<int, std::pair<std::map<std::string, int>, int>> enemyLoot;
	std::map<int, std::pair<std::map<std::string, int>, int>> chestLoot;
	std::vector<LevelExitBean> levelExits;
	sf::FloatRect levelRect;
};