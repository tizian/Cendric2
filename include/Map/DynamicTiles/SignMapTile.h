#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"
#include "Structs/SignData.h"
#include "GUI/TooltipWindow.h"

class InteractComponent;

// a readable sign
class SignMapTile final : public MapDynamicTile {
public:
	SignMapTile(const SignData& data, MapScreen* mapScreen);

	void update(const sf::Time& frameTime) override;
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;

	void init() override;
	void loadAnimation(int skinNr) override;
	void setPosition(const sf::Vector2f& pos) override;
	void onMouseOver() override;

	GameObjectType getConfiguredType() const override;
	MapDynamicTileID getDynamicTileID() const override { return MapDynamicTileID::Sign; }

private:
	std::string getSpritePath() const override;
	void onInteract();

private:
	SignData m_data;
	TooltipWindow m_tooltipWindow;
	bool m_showTooltip;
	sf::Time m_tooltipWindowTime = sf::Time::Zero;
	InteractComponent* m_interactComponent;

	static const sf::Time TOOLTIP_WINDOW_TIME;
	static const float READ_RANGE;
	static const float TOOLTIP_TOP;
};