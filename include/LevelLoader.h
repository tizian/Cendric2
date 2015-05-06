#pragma once

#include <vector>

#include "global.h"
#include "Screen.h"
#include "LevelReader.h"
#include "DynamicTile.h"
#include "DynamicTiles\WaterTile.h"
#include "DynamicTiles\IceTile.h"
#include "DynamicTiles\CrumblyBlockTile.h"
#include "LevelItem.h"
#include "TextProvider.h"

// TODO maybe this information should come over files rather than be hardcoded here.
struct LevelItemBean
{
	sf::Vector2f spriteOffset;
	sf::FloatRect boundingBox;
	ResourceID textureID;
	sf::IntRect texturePos;
	sf::Time frameTime;
	Texts tooltip;
};

// helper class used by the level to load dynamic tiles, level items and enemies
class LevelLoader
{
public:
	// loads dynamic tiles out of level data and adds them directly to the screen
	void loadDynamicTiles(LevelData& data, Screen* screen) const;
	// loads level items out of level data and adds them directly to the screen
	void loadLevelItems(LevelData& data, Screen* screen) const;
	// loads enemies out of level data and adds them directly to the screen
	void loadEnemies(LevelData& data, Screen* screen) const;
};