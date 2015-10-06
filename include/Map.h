#pragma once

#include <vector>
#include <math.h>

#include "global.h"
#include "TileMap.h"
#include "FileIO/MapReader.h"
#include "MapLoader.h"
#include "ResourceManager.h"

class Map {
public:
	Map();
	~Map();

	// loads a .dricmap file
	bool load(const std::string& id);
	void loadAfterMainChar(Screen* screen);
	// loads lights. this happens AFTER everything else and is because of our nice RENDERTEXTURE PROBLEM >:(
	void loadForRenderTexture(Screen* screen);
	// draws the map. background tilelayers
	void drawBackground(sf::RenderTarget& target, const sf::RenderStates states, const sf::Vector2f& center) const;
	// draws the map. foreground tilelayers
	void drawForeground(sf::RenderTarget& target, const sf::RenderStates states, const sf::Vector2f& center) const;
	// deletes the resources
	void dispose();

	// checks collision with the collidable grid of that map
	bool collidesX(const sf::FloatRect& boundingBox) const;
	bool collidesY(const sf::FloatRect& boundingBox) const;
	// checks if the main char has reached a level entry. If no, nullptr, else the spawn point and id for that level
	MapExitBean* checkLevelEntry(const sf::FloatRect& boundingBox) const;

	const TileMap& getBackgroundTilemap() const;
	const TileMap& getForegroundTilemap() const;
	const sf::FloatRect& getMapRect() const;
	const std::string& getID() const;
	// returns the global dimming factor (0 to 1)
	float getDimming() const;


private:
	void draw(sf::RenderTarget& target, const sf::RenderStates states, const sf::Vector2f& center, const TileMap& map) const;
	TileMap m_backgroundTileMap;
	TileMap m_foregroundTileMap;

	std::string m_id;
	// data loaded by the map loader
	MapData m_mapData;
};