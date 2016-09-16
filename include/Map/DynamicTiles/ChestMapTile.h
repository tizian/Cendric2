#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"

// a chest/chest like object that can be looted on the map, it is currently only used to retrieve stored data.
class ChestMapTile : public MapDynamicTile {
public:
	ChestMapTile(MapScreen* mapScreen);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onRightClick() override;
	void onLeftClick() override;
	MapDynamicTileID getDynamicTileID() const override { return MapDynamicTileID::Chest; }

private:
	void loot();

private:
	std::string getSpritePath() const override;
	static const float RANGE;
};