#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class InteractComponent;
class TooltipWindowComponent;

class SignLevelTile final : public LevelDynamicTile {
public:
	SignLevelTile(LevelScreen* levelScreen);

	void update(const sf::Time& frameTime) override;

	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override {}
	void onMouseOver() override;

	GameObjectType getConfiguredType() const override { return LevelDynamicTile::getConfiguredType(); }
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Sign; }

private: 
	void onInteract();

private:
	std::string getSpritePath() const override;

	InteractComponent* m_interactComponent;
	TooltipWindowComponent* m_tooltipComponent;

	static const float READ_RANGE;
	static const float TOOLTIP_TOP;
};