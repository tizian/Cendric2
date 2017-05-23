#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"
#include "World/ChestTile.h"

class InteractComponent;

// a chest/chest like object that can be looted on the map, it is currently only used to retrieve stored data.
class ChestMapTile final : public MapDynamicTile, public ChestTile {
public:
	ChestMapTile(MapScreen* mapScreen);
	bool init(const MapTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onRightClick() override;
	void onLeftClick() override;
	MapDynamicTileID getDynamicTileID() const override { return MapDynamicTileID::Chest; }

private:
	void loot();
	void unlock();

private:
	std::string getSpritePath() const override;
	static const float PICKUP_RANGE;
};