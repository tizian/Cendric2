#pragma once

#include "global.h"
#include "World/TileMap.h"
#include "ResourceManager.h"
#include "Logger.h"
#include "Structs/WorldData.h"
#include "Structs/WorldCollisionQueryRecord.h"

class MainCharacter;

// a level or a map in the cendric world
class World {

public:
	World();
	virtual ~World();

	// updates all tilemaps
	virtual void update(const sf::Time& frameTime);

	virtual bool load(const std::string& id, WorldScreen* screen) = 0;
	virtual void loadAfterMainChar(MainCharacter* mainChar) = 0;
	// loads lights. this happens AFTER everything else and is because of our nice RENDERTEXTURE PROBLEM >:(
	virtual void loadForRenderTexture() = 0;
	// sets the target view to the correct world view with the focus center focus.
	virtual void setWorldView(sf::RenderTarget& target, const sf::Vector2f& focus) const = 0;
	// draws the world. background tilelayers.
	void drawBackground(sf::RenderTarget& target, const sf::RenderStates& states) const;
	// draws the world. lighted foreground tilelayers
	void drawLightedForeground(sf::RenderTarget& target, const sf::RenderStates& states) const;
	// draws the world. foreground tilelayers
	virtual void drawForeground(sf::RenderTarget& target, const sf::RenderStates& states) const;

	const TileMap* getBackgroundTileMap() const; 
	const TileMap* getLightedForegroundTileMap() const;
	const TileMap* getForegroundTileMap() const;

	virtual bool collides(WorldCollisionQueryRecord& rec) const;
	bool collidesWithCollidableLayer(const sf::Vector2f& pos) const;
	
	// deletes the world resources
	virtual void dispose();

	const sf::FloatRect& getWorldRect() const;
	const std::string& getID() const;
	const WeatherData& getWeather() const;
	const std::string& getMusicPath() const;
	std::string getName() const;
	static std::string getNameFromId(const std::string id);
	virtual const WorldData* getWorldData() const = 0;

	void setAmbientDimming(float dimming) const;
	void setLightDimming(float dimming) const;

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