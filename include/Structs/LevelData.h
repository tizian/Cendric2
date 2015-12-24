#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "BackgroundLayer.h"
#include "Structs/TMXData.h"
#include "Structs/LevelExitData.h"
#include "Structs/LevelDynamicTileData.h"
#include "Structs/LeverData.h"

struct LevelData : TMXData {
	std::vector<BackgroundLayer> backgroundLayers;
	std::vector<std::pair<LevelDynamicTileID, std::vector<int>>> dynamicTileLayers;
	std::vector<LevelDynamicTileData> dynamicTiles;
	std::vector<AnimatedTileData> animatedTiles;
	std::vector<LeverData> levers;
	std::vector<std::string> levelItems;

	std::map<int, std::pair<EnemyID, sf::Vector2f>> enemies;
	std::map<int, std::pair<int, sf::Vector2f>> chests;
	std::map<int, int> chestStrength;

	std::map<int, std::pair<std::string, std::string>> enemyQuesttarget;
	std::map<int, std::pair<std::map<std::string, int>, int>> enemyLoot;
	std::map<int, std::pair<std::map<std::string, int>, int>> chestLoot;
	std::vector<LevelExitData> levelExits;
};