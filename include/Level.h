#pragma once

#include <vector>
#include <math.h>

#include "global.h"
#include "TileMap.h"
#include "BackgroundLayer.h"
#include "LevelReader.h"
#include "ResourceManager.h"
#include "DynamicTile.h"
#include "Logger.h"
#include "LevelLoader.h"

#include "Enums/LevelID.h"

class Level 
{
public:
	Level();
	~Level();

	// loads a .dric file
	bool load(LevelID id, Screen* screen);
	// draws the level. Background layers, tilemap, dynamic tiles, foreground layers.
	void draw(sf::RenderTarget &target, sf::RenderStates states, const sf::Vector2f& center) const;
	// deletes the resources
	void dispose();

	// checks collision with the collidable grid of that level
	bool collidesX(const sf::FloatRect& boundingBox) const;
	bool collidesY(const sf::FloatRect& boundingBox) const;
	void collideWithDynamicTiles(Spell* spell, const sf::FloatRect& nextBoundingBoxX, const sf::FloatRect& nextBoundingBoxY) const;

	const TileMap& getTilemap() const;
	const sf::Vector2f& getStartPos() const;
	const sf::FloatRect& getLevelRect() const;
	LevelID getID() const;
	// returns the next y position where the bounding box is grounding.
	float getGround(const sf::FloatRect& boundingBox) const;

private:
	TileMap m_tileMap;
	sf::FloatRect m_levelRect;
	std::vector<std::vector<bool>> m_collidableTiles;
	std::vector<BackgroundLayer> m_backgroundLayers;
	std::vector<GameObject*>* m_dynamicTiles;
	std::string m_name;
	sf::Vector2f m_startPos;
	LevelID m_id;
};