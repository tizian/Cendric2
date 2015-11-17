#include "DynamicTiles/SimulatedWaterTileData.h"

SimulatedWaterTileData SimulatedWaterTileData::getData(int skinNr) {
	switch (skinNr) {
	case 1:
		return WATER;
	case 2:
		return LAVA;
	default:
		g_logger->logError("SimulatedWaterTileData", "Could not find parameters for water skin with skin nr " + std::to_string(skinNr));
		return WATER;
	}
}