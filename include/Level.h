#pragma once

#include <vector>
#include <math.h>

#include "global.h"
#include "TileMap.h"
#include "BackgroundLayer.h"
#include "LevelReader.h"
#include "ResourceManager.h"
#include "DynamicTile.h"
#include "LevelItem.h"
#include "Logger.h"
#include "LevelLoader.h"

class Level 
{
public:
	Level();
	~Level();

	// getters
	TileMap getTilemap();
	sf::Vector2f& getStartPos();
	sf::FloatRect& getLevelRect();

	// checks collision with the collidable grid of that level
	bool collidesX(const sf::FloatRect& boundingBox);
	bool collidesY(const sf::FloatRect& boundingBox);
	void collideWithDynamicTiles(Spell* spell, const sf::FloatRect nextBoundingBoxX, const sf::FloatRect nextBoundingBoxY);
	// returns the next y position where the bounding box is grounding.
	float getGround(const sf::FloatRect& boundingBox);

	// loads a .dric file
	bool load(ResourceID id, Screen* screen);

	// deletes the resources
	void dispose();

	// draws the level. Background layers, tilemap, dynamic tiles, foreground layers.
	void draw(sf::RenderTarget &target, sf::RenderStates states, const sf::Vector2f& center);

private:
	TileMap m_tileMap;
	sf::FloatRect m_levelRect;
	std::vector<std::vector<bool>> m_collidableTiles;
	std::vector<BackgroundLayer> m_backgroundLayers;
	std::vector<GameObject*>* m_dynamicTiles;
	std::string m_name;
	sf::Vector2f m_startPos;
	ResourceID m_resourceID;
};