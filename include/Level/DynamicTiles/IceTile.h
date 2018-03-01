#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class IceTile final : public LevelDynamicTile {
public:
	IceTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	bool init(const LevelTileProperties& properties) override;
	void update(const sf::Time& frameTime) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void setDisposed() override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Ice; }

private:
	std::string getSpritePath() const override;
	std::string getSoundPath() const override;
	bool m_isFreezing;
};