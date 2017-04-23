#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"
#include "Structs/ChestTileData.h"

class InteractComponent;

// a chest/chest like object that can be looted on the map, it is currently only used to retrieve stored data.
class ChestMapTile final : public MapDynamicTile {
public:
	ChestMapTile(MapScreen* mapScreen);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onRightClick() override;
	void onLeftClick() override;
	MapDynamicTileID getDynamicTileID() const override { return MapDynamicTileID::Chest; }

	void setChestData(const ChestTileData& data);

private:
	void loot();
	void unlock();

private:
	ChestTileData m_data;
	InteractComponent* m_interactComponent = nullptr;
	std::string getSpritePath() const override;
	static const float PICKUP_RANGE;
};