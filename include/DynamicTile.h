#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Enums/DynamicTileID.h"

class Spell;
class LevelMovableGameObject;

// A dynamic tile in a level
class DynamicTile : public GameObject
{
public:
	DynamicTile() : GameObject() {}
	// loads sprite offset & bounding box. maybe uses skin nr later, too.
	virtual void init() = 0;
	// loads the dynamic tile with the specified skin nr
	virtual void load(int skinNr) = 0;
	virtual void update(const sf::Time& frameTime) override;
	// gets called by a spell when it hits the dynamic tile
	virtual void onHit(Spell* spell) = 0;
	// gets called by a mob when it hits the dynamic tile. default does nothing
	virtual void onHit(LevelMovableGameObject* mob);
	
	// gets called by the level when initializing the tile
	void setTileSize(const sf::Vector2i tileSize);
	
	virtual bool getIsCollidable() const;
	GameObjectType getConfiguredType() const override;

protected:
	// dynamic tile textures have a border (border width in pixel)
	const int BORDER = 1;
	sf::Vector2i m_tileSize;
	bool m_isCollidable;
	
private:	
	
};