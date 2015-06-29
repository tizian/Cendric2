#pragma once

#include <vector>

#include "global.h"
#include "Screen.h"

#include "Enums/NpcID.h"

#include "Structs/MapData.h"
#include "Structs/NPCBean.h"

class Map;

// helper class used by the map to load npcs
class MapLoader
{
public:
	// loads npcs out of map data and adds them directly to the screen
	void loadNpcs(MapData& data, Screen* screen) const;
};