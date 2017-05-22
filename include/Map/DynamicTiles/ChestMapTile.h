#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"

class InteractComponent;

// a chest/chest like object that can be looted on the map, it is currently only used to retrieve stored data.
class ChestMapTile final : public MapDynamicTile {
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
	InteractComponent* m_interactComponent = nullptr;
	std::string getSpritePath() const override;
	static const float PICKUP_RANGE;

private:
	int m_chestStrength = 0;
	bool m_isStoredItems = false;
	std::string m_keyItemId;
	std::map<std::string, int> m_lootableItems;
	int m_lootableGold = 0;
	bool m_isPermanent = false;
};