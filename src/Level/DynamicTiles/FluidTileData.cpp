#include "Level/DynamicTiles/FluidTileData.h"

FluidTileData FluidTileData::getData(int skinNr) {
	switch (skinNr) {
	case 0:
		return WATER;
	case 1:
		return LAVA;
	case 2:
		return SHALLOW_WATER;
	case 3:
		return POISON;
	case 4:
		return FREEZING_WATER;
	default:
		g_logger->logError("FluidTileData", "Could not find parameters for fluid skin with skin nr " + std::to_string(skinNr));
		return WATER;
	}
}