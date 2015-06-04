#pragma once

#include <vector>
#include <math.h>

#include "global.h"
#include "TileMap.h"
#include "FileIO/MapReader.h"
#include "ResourceManager.h"

class Map
{
public:
	Map();
	~Map();
	
	// loads a .dricmap file
	bool load(MapID id);
	// draws the map. tilelayers, objects, cendric.
	void draw(sf::RenderTarget& target, const sf::RenderStates states, const sf::Vector2f& center) const;
	// deletes the resources
	void dispose();
	
	// checks collision with the collidable grid of that map
	bool collidesX(const sf::FloatRect& boundingBox) const;
	bool collidesY(const sf::FloatRect& boundingBox) const;
	// checks if the main char has reached a level entry. If no, it returns the void resource id, else the resource id of that level
	LevelID checkLevelEntry(const sf::FloatRect& boundingBox) const;

	const TileMap& getTilemap() const;
	const sf::Vector2f& getStartPos() const;
	const sf::FloatRect& getMapRect() const;
	MapID getID() const;


private:
	TileMap m_tileMap;
	sf::FloatRect m_mapRect;
	std::vector<std::vector<bool>> m_collidableTiles;
	std::string m_name;
	sf::Vector2f m_startPos;

	MapID m_id;
};