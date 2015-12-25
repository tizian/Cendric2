#pragma once

#include "global.h"
#include "MovableGameObject.h"

class Map;

// a MOB in a map, npcs + main character.
class MapMovableGameObject : virtual public MovableGameObject {
public:
	MapMovableGameObject(Map* map);
	virtual ~MapMovableGameObject();
	
	Map* getMap() const;
	GameObjectState getState() const;

protected:
	// update animation based on the current velocity
	virtual void updateAnimation(const sf::Time& frameTime);
	Map* m_map;
};