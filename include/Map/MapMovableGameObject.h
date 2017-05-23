#pragma once

#include "global.h"
#include "World/MovableGameObject.h"

class Map;

// a MOB in a map, npcs + main character.
class MapMovableGameObject : public virtual MovableGameObject {
public:
	MapMovableGameObject(const Map* map);
	virtual ~MapMovableGameObject();
	
	const Map* getMap() const;
	
protected:
	// update animation based on the current velocity
	virtual void updateAnimation(const sf::Time& frameTime);
	const Map* m_map;
};