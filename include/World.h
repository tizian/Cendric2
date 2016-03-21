#pragma once

#include <vector>
#include <math.h>

#include "global.h"
#include "TileMap.h"
#include "ResourceManager.h"
#include "Logger.h"
#include "Structs/WorldData.h"
#include "Structs/WorldCollisionQueryRecord.h"

// a level or a map in the cendric world
class World {

public:
	World();
	virtual ~World();

	// updates all tilemaps
	virtual void update(const sf::Time& frameTime);

	virtual bool load(const std::string& id) = 0;
	virtual void loadAfterMainChar(WorldScreen* screen) = 0;
	// loads lights. this happens AFTER everything else and is because of our nice RENDERTEXTURE PROBLEM >:(
	virtual void loadForRenderTexture(WorldScreen* screen) = 0;
	// sets the target view to the correct world view with the focus center focus.
	virtual void setWorldView(sf::RenderTarget& target, const sf::Vector2f& focus) const = 0;
	// draws the world. background tilelayers.
	void drawBackground(sf::RenderTarget& target, const sf::RenderStates& states) const;
	// draws the world. lighted foreground tilelayers
	void drawLightedForeground(sf::RenderTarget& target, const sf::RenderStates& states) const;
	// draws the world. foreground tilelayers
	void drawForeground(sf::RenderTarget& target, const sf::RenderStates& states) const;

	const TileMap* getBackgroundTileMap() const; 
	const TileMap* getLightedForegroundTileMap() const;
	const TileMap* getForegroundTileMap() const;

	virtual bool collides(WorldCollisionQueryRecord& rec) const;
	
	// deletes the world resources
	virtual void dispose();

	const sf::FloatRect& getWorldRect() const;
	const std::string& getID() const;
	float getDimming() const;
	const std::string& getMusicPath() const;
	const std::string& getName() const;

protected:
	// this pointer is set by the descendants (map and level)
	WorldData* m_worldData;

	TileMap m_backgroundTileMap;
	TileMap m_lightedForegroundTileMap;
	TileMap m_foregroundTileMap;

	Screen* m_screen;

	// helper method to calculate safe collision locations, regarding the collider bounding box 'bb'
	// the record is updated with this safe values
	void calculateCollisionLocations(WorldCollisionQueryRecord& rec, const sf::FloatRect& bb) const;
};