#pragma once

#include "World.h"
#include "FileIO/MapReader.h"
#include "MapLoader.h"

// a top down level
class Map : public World {
public:
	Map();
	~Map();

	bool load(const std::string& id, WorldScreen* screen) override;
	void loadAfterMainChar() override;
	// loads lights. this happens AFTER everything else and is because of our nice RENDERTEXTURE PROBLEM >:(
	void loadForRenderTexture() override;
	void setWorldView(sf::RenderTarget& target, const sf::Vector2f& focus) const override;

	void drawFogOfWar(sf::RenderTarget& target, const sf::RenderStates& states) const;

	const MapData* getWorldData() const;
	
	// checks collision with the collidable grid of that map
	bool collides(WorldCollisionQueryRecord& rec) const override;
	void dispose() override;

private:
	// data loaded by the map loader
	MapData m_mapData;

	std::vector<GameObject*>* m_dynamicTiles;
};