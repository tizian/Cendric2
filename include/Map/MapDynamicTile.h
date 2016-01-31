#pragma once

#include "global.h"
#include "AnimatedGameObject.h"
#include "ResourceManager.h"
#include "Enums/MapDynamicTileID.h"

class Map;
class MapMainCharacter;

// A dynamic tile in a map
class MapDynamicTile : public virtual AnimatedGameObject {
public:
	MapDynamicTile(Map* map);
	virtual ~MapDynamicTile() {};

	// loads sprite offset & bounding box. 
	virtual void init() = 0;
	// loads the dynamic tile with the specified skin nr
	virtual void loadAnimation(int skinNr) = 0;
	// gets called by the main character when it hits the dynamic tile. default does nothing
	virtual void onHit(MapMainCharacter* mob);

	// sets the position offset for a dynamic tile. DON'T use that for collidable dynamic tiles.
	void setPositionOffset(const sf::Vector2f& offset);

	const sf::Vector2f& getPositionOffset() const;
	virtual bool getIsCollidable() const;
	GameObjectType getConfiguredType() const override;

protected:
	// dynamic tile textures have a border (border width in pixel)
	const int BORDER = 1;
	bool m_isCollidable = false;
	Map* m_map;

private:
	sf::Vector2f m_positionOffset = sf::Vector2f(0.f, 0.f);
};