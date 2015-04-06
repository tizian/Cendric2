#pragma once

#include <vector>
#include <math.h>

#include "global.h"
#include "TileMap.h"
#include "MapReader.h"
#include "ResourceManager.h"

class Map
{
public:
	Map();
	~Map();

	// getters
	TileMap getTilemap();
	sf::Vector2f& getStartPos();
	sf::FloatRect& getMapRect();

	// checks collision with the collidable grid of that map
	bool collidesX(const sf::FloatRect& boundingBox);
	bool collidesY(const sf::FloatRect& boundingBox);

	// checks if the main char has reached a level entry. If no, it returns the void resource id, else the resource id of that level
	ResourceID checkLevelEntry(const sf::FloatRect& boundingBox);

	// loads a .dricmap file
	bool load(ResourceID id);

	// deletes the resources
	void dispose();

	// draws the map. tilelayers, objects, cendric.
	void draw(sf::RenderTarget &target, sf::RenderStates states, const sf::Vector2f& center);

private:
	TileMap m_tileMap;
	sf::FloatRect m_mapRect;
	std::vector<std::vector<bool>> m_collidableTiles;
	std::string m_name;
	sf::Vector2f m_startPos;

	ResourceID m_resourceID;
};