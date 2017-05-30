#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"
#include "GUI/TooltipWindow.h"
#include "Structs/Condition.h"

class InteractComponent;

// a door
class DoorMapTile final : public MapDynamicTile {
public:
	DoorMapTile(MapScreen* mapScreen);

	void update(const sf::Time& frameTime) override;

	bool init(const MapTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onRightClick() override;

	void notifyReloadNeeded();
	
	MapDynamicTileID getDynamicTileID() const override { return MapDynamicTileID::Door; }

private:
	std::string getSpritePath() const override;
	void reloadConditions();
	void open();
	void close();

private:
	InteractComponent* m_interactComponent;
	bool m_isOpen;
	bool m_isReloadNeeded = false;
	bool m_isConditionsFulfilled;

	static const float OPEN_RANGE;

private:
	std::string m_keyItemId;
	std::vector<Condition> m_conditions;
};