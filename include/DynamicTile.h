#pragma once

#include "global.h"
#include "GameObject.h"
#include "ResourceManager.h"

class Spell;

enum class DynamicTileID
{
	Void,
	Water,
	Ice,
	Crumbly_block
};

// A dynamic tile in a level
class DynamicTile : public GameObject
{
public:

	virtual void update(sf::Time& frameTime) override;

	virtual bool getIsCollidable();
	// gets called by a spell when it hits the dynamic tile
	virtual void onHit(Spell* spell) = 0;
	// gets called by the level when initializing the tile
	void setTileSize(const sf::Vector2i tileSize);
	GameObjectType getConfiguredType() const override;

protected:
	// dynamic tile textures have a border (border width in pixel)
	const int BORDER = 1;
	sf::Vector2i m_tileSize;
	bool m_isCollidable;
	
private:	
	
};