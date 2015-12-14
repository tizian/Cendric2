#pragma once

#include <vector>

#include "global.h"
#include "Screen.h"

#include "Structs/MapData.h"
#include "Structs/NPCBean.h"
#include "Enums/MapDynamicTileID.h"

#include "Map/DynamicTiles/CookingTile.h"

class Map;

// helper class used by the map to load npcs
class MapLoader {
public:
	// loads dynamic tiles out of map data and adds them directly to the screen.
	void loadDynamicTiles(MapData& data, Screen* screen, Map* map) const;
	// loads npcs out of map data and adds them directly to the screen
	void loadNpcs(MapData& data, Screen* screen) const;
	// loads lights out of map data and adds them directly to the screen
	void loadLights(MapData& data, Screen* screen) const;
};