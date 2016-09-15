#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"
#include "Structs/DoorData.h"
#include "GUI/TooltipWindow.h"

class InteractComponent;

// a door
class DoorMapTile : public MapDynamicTile {
public:
	DoorMapTile(const DoorData& data, MapScreen* mapScreen);

	void update(const sf::Time& frameTime) override;
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;

	void init() override;
	void loadAnimation(int skinNr) override;
	void setPosition(const sf::Vector2f& pos) override;
	void onMouseOver() override;
	void onRightClick() override;

	void notifyReloadNeeded();
	
	MapDynamicTileID getDynamicTileID() const override { return MapDynamicTileID::Door; }

private:
	std::string getSpritePath() const override;
	void reloadConditions();
	void open();

private:
	DoorData m_data;
	InteractComponent* m_interactComponent;
	bool m_open;
	bool m_reloadNeeded = false;
	bool m_conditionsFulfilled;

	static const float OPEN_RANGE;
	static const float TOOLTIP_TOP;
};