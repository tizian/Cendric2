#pragma once

#include <vector>
#include <map>

#include "global.h"
#include "Screen.h"
#include "LevelMainCharacter.h"
#include "Level.h"
#include "LevelEquipment.h"

#include "Structs/LevelEquipmentData.h"

// helper class used by the game screan to load the main character including his weapon / equipment.
class LevelMainCharacterLoader {
public:
	// loads the main character and adds it directly to the screen
	LevelMainCharacter* loadMainCharacter(Screen* screen, Level* level) const;
	// loads level items out of level data and adds them directly to the screen
	void loadEquipment(Screen* screen) const;
};