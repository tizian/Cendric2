#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "BackgroundLayer.h"
#include "Structs/LevelExitBean.h"
#include "Structs/DynamicTileBean.h"
#include "Structs/LightBean.h"
#include "Structs/LeverBean.h"

struct LevelData {
	std::string id;
	std::string name;
	sf::Vector2i mapSize;
	sf::Vector2i tileSize;
	std::string tileSetPath;
	std::string musicPath;
	std::vector<BackgroundLayer> backgroundLayers;
	std::vector<std::vector<int>> backgroundTileLayers;
	std::vector<std::vector<int>> lightedForegroundTileLayers;
	std::vector<std::vector<int>> foregroundTileLayers;
	std::vector<bool> collidableTiles;
	std::vector<std::vector<bool>> collidableTilePositions;
	std::vector<std::pair<DynamicTileID, std::vector<int>>> dynamicTileLayers;
	std::vector<DynamicTileBean> dynamicTiles;
	std::vector<LeverBean> levers;
	std::vector<std::string> levelItems;
	std::map<int, std::pair<EnemyID, sf::Vector2f>> enemies;
	std::map<int, std::pair<int, sf::Vector2f>> chests;
	std::map<int, int> chestStrength;
	std::map<int, std::pair<std::string, std::string>> enemyQuesttarget;
	std::map<int, std::pair<std::map<std::string, int>, int>> enemyLoot;
	std::map<int, std::pair<std::map<std::string, int>, int>> chestLoot;
	std::vector<LevelExitBean> levelExits;
	std::vector<LightBean> lights;
	sf::FloatRect levelRect;
	float dimming = 0.f;
};