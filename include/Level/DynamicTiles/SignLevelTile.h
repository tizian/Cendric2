#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Structs/SignData.h"
#include "GUI/TooltipWindow.h"

class SignLevelTile : public virtual LevelDynamicTile {
public:
	SignLevelTile(const SignData& data, LevelScreen* levelScreen);

	void update(const sf::Time& frameTime) override;
	void renderAfterForeground(sf::RenderTarget& renderTarget) override;

	void init() override;
	void loadAnimation(int skinNr) override;
	void setPosition(const sf::Vector2f& pos) override;
	void onMouseOver() override;
	void onHit(Spell* spell) override {}

	GameObjectType getConfiguredType() const override { return LevelDynamicTile::getConfiguredType(); }

private:
	SignData m_data;
	TooltipWindow m_tooltipWindow;
	bool m_showTooltip;
};