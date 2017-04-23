#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"
#include "Structs/DoorData.h"
#include "GUI/TooltipWindow.h"

class InteractComponent;

// a door
class DoorMapTile final : public MapDynamicTile {
public:
	DoorMapTile(MapScreen* mapScreen);

	void update(const sf::Time& frameTime) override;

	void init() override;
	void loadAnimation(int skinNr) override;
	void onRightClick() override;

	void notifyReloadNeeded();
	void setDoorData(const DoorData& data);
	
	MapDynamicTileID getDynamicTileID() const override { return MapDynamicTileID::Door; }

private:
	std::string getSpritePath() const override;
	void reloadConditions();
	void open();
	void close();

private:
	DoorData m_data;
	InteractComponent* m_interactComponent;
	bool m_isOpen;
	bool m_isReloadNeeded = false;
	bool m_isConditionsFulfilled;

	static const float OPEN_RANGE;
};