#pragma once

#include <vector>
#include <map>

#include "global.h"
#include "Screens/Screen.h"
#include "Level/LevelMainCharacter.h"
#include "Level/Level.h"
#include "Level/LevelEquipment.h"

#include "Structs/LevelEquipmentData.h"

// helper class used by the game screan to load the main character including his weapon / equipment.
class LevelMainCharacterLoader {
public:
	// loads the main character and adds it directly to the screen
	static LevelMainCharacter* loadMainCharacter(Screen* screen, Level* level);
	// loads level equipment adds it directly to the screen
	static void loadEquipment(Screen* screen);
};