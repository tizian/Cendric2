#pragma once

#include <vector>

#include "global.h"
#include "Screen.h"

#include "Structs/MapData.h"
#include "Structs/NPCData.h"
#include "Enums/MapDynamicTileID.h"

#include "Map/DynamicTiles/CookingTile.h"

class MapScreen;

// helper class used by the map to load npcs
class MapLoader {
public:
	// loads dynamic tiles out of map data and adds them directly to the screen.
	void loadDynamicTiles(MapData& data, MapScreen* screen) const;
	// loads npcs out of map data and adds them directly to the screen
	void loadNpcs(MapData& data, MapScreen* screen) const;
	// loads lights out of map data and adds them directly to the screen
	void loadLights(MapData& data, MapScreen* screen) const;
	// loads triggers out of map data and adds them directly to the screen
	void loadTriggers(MapData& data, MapScreen* screen) const;
};