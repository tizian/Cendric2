#pragma once

#include <vector>
#include <map>

#include "global.h"
#include "Screens/Screen.h"
#include "Map/MapMainCharacter.h"
#include "Map/Map.h"
#include "Map/MapEquipment.h"

// helper class used by the game screan to load the main character including his weapon / equipment.
class MapMainCharacterLoader {
public:
	// loads the main character and adds it directly to the screen
	MapMainCharacter* loadMainCharacter(Screen* screen, Map* map) const;
	// loads map equipment adds it directly to the screen
	void loadEquipment(Screen* screen) const;
};