#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"

class InteractComponent;
class TooltipWindowComponent;

// a readable sign
class SignMapTile final : public MapDynamicTile {
public:
	SignMapTile(MapScreen* mapScreen);

	void update(const sf::Time& frameTime) override;

	bool init(const MapTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onMouseOver() override;

	GameObjectType getConfiguredType() const override;
	MapDynamicTileID getDynamicTileID() const override { return MapDynamicTileID::Sign; }

private:
	std::string getSpritePath() const override;
	void onInteract();

private:
	bool m_isBackground;
	InteractComponent* m_interactComponent;
	TooltipWindowComponent* m_tooltipComponent;

	static const float READ_RANGE;
	static const float TOOLTIP_TOP;
};