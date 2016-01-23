#pragma once

#include <vector>
#include <math.h>

#include "global.h"
#include "TileMap.h"
#include "ResourceManager.h"
#include "Logger.h"
#include "Structs/WorldData.h"

// a level or a map in the cendric world
class World {

public:
	World();
	virtual ~World();

	// updates all tilemaps
	virtual void update(const sf::Time& frameTime);

	virtual bool load(const std::string& id) = 0;
	virtual void loadAfterMainChar(Screen* screen) = 0;
	// loads lights. this happens AFTER everything else and is because of our nice RENDERTEXTURE PROBLEM >:(
	virtual void loadForRenderTexture(Screen* screen) = 0;
	// sets the target view to the correct world view with the focus center focus.
	virtual void setWorldView(sf::RenderTarget& target, const sf::Vector2f& focus) const = 0;
	// draws the world. background tilelayers.
	void drawBackground(sf::RenderTarget& target, const sf::RenderStates& states) const;
	// draws the world. lighted foreground tilelayers
	void drawLightedForeground(sf::RenderTarget& target, const sf::RenderStates& states) const;
	// draws the world. foreground tilelayers
	void drawForeground(sf::RenderTarget& target, const sf::RenderStates& states) const;

	// checks collision with the collidable grid of that world and also the collidable dynamic tiles
	// if the calling object itself wants to be excluded, it can give itself as an argument here.
	// the last argument will ignore collidable dynamic tiles in the check, but no strictly collidable dynamic tiles.
	virtual bool collides(const sf::FloatRect& boundingBox, const GameObject* exclude = nullptr, bool ignoreDynamicTiles = false, bool ignoreMobs = true) const;
	// returns the first non-colliding location in the specific direction, floored.
	// these locations refer to the top left location of the bounding box.
	float getNonCollidingTop(const sf::FloatRect& boundingBox, const GameObject* exclude = nullptr, bool ignoreDynamicTiles = false, bool ignoreMobs = true) const;
	float getNonCollidingBottom(const sf::FloatRect& boundingBox, const GameObject* exclude = nullptr, bool ignoreDynamicTiles = false, bool ignoreMobs = true) const;
	float getNonCollidingLeft(const sf::FloatRect& boundingBox, const GameObject* exclude = nullptr, bool ignoreDynamicTiles = false, bool ignoreMobs = true) const;
	float getNonCollidingRight(const sf::FloatRect& boundingBox, const GameObject* exclude = nullptr, bool ignoreDynamicTiles = false, bool ignoreMobs = true) const;
	
	// deletes the resources
	virtual void dispose();

	const sf::FloatRect& getWorldRect() const;
	const std::string& getID() const;
	float getDimming() const;
	const std::string& getMusicPath() const;

protected:
	// this pointer is set by the descendants (map and level)
	WorldData* m_worldData;

	TileMap m_backgroundTileMap;
	TileMap m_lightedForegroundTileMap;
	TileMap m_foregroundTileMap;

	Screen* m_screen;

	float tileHeight;
	float tileWidth;
};