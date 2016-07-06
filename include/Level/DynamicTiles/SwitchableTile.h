#pragma once

#include "global.h"
#include "Level/DynamicTiles/LeverDependentTile.h"

class SwitchableTile : public virtual LeverDependentTile {
public:
	SwitchableTile(LevelScreen* levelScreen) : 
		LevelDynamicTile(levelScreen), 
		LeverDependentTile(levelScreen) {}
	void init() override;
	void setInitialState(bool on) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override { /*nop*/ };

	void switchTile() override;
	bool isSwitchable() const override;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::SwitchableOn; }

private:
	std::string getSpritePath() const override;
};