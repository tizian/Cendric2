#pragma once

#include "global.h"
#include "GUI/GameInterface.h"

// The interface overlay in a map
// if the keys for character screen, spellbook and inventory are pressed, these will display.
class MapInterface : public GameInterface
{
public:
	MapInterface(GameScreen* screen);
	~MapInterface();

private:
};