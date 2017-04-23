#pragma once

#include "global.h"
#include "GUI/WorldInterface.h"

// The interface overlay in a map
// if the keys for character screen, spellbook and inventory are pressed, these will display.
class MapInterface final : public WorldInterface {
public:
	MapInterface(WorldScreen* screen);
	~MapInterface();
};