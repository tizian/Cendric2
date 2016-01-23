#pragma once

#include "World.h"
#include "FileIO/MapReader.h"
#include "MapLoader.h"

// a top down level
class Map : public World {
public:
	Map();
	~Map();

	bool load(const std::string& id);
	void loadAfterMainChar(Screen* screen);
	// loads lights. this happens AFTER everything else and is because of our nice RENDERTEXTURE PROBLEM >:(
	void loadForRenderTexture(Screen* screen);
	void setWorldView(sf::RenderTarget& target, const sf::Vector2f& focus) const override;
	
	// checks collision with the collidable grid of that map
	bool collides(const sf::FloatRect& boundingBox, const GameObject* exclude = nullptr, bool ignoreDynamicTiles = false, bool ignoreMobs = true) const override;
	// checks if the main char has reached a level entry. If no, nullptr, else the spawn point and id for that level
	MapExitData* checkLevelEntry(const sf::FloatRect& boundingBox) const;

private:
	// data loaded by the map loader
	MapData m_mapData;
};