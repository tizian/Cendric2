#pragma once

#include "global.h"
#include "Level/DynamicTiles/LeverDependentTile.h"

class SwitchableTile : public virtual LeverDependentTile {
public:
	SwitchableTile(LevelScreen* levelScreen) : 
		LevelDynamicTile(levelScreen), 
		LeverDependentTile(levelScreen) {}
	virtual ~SwitchableTile() {}
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override { /*nop*/ };

	void switchTile() override;
	bool isSwitchable() const override;

protected:
	void setInitialState(bool on) override;

private:
	std::string getSpritePath() const override;
};

class SwitchableOnTile final : public SwitchableTile {
public:
	SwitchableOnTile(LevelScreen* levelScreen) :
		LevelDynamicTile(levelScreen),
		LeverDependentTile(levelScreen),
		SwitchableTile(levelScreen) {
		setInitialState(true);
	}

	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::SwitchableOn; }
};

class SwitchableOffTile final : public SwitchableTile {
public:
	SwitchableOffTile(LevelScreen* levelScreen) :
		LevelDynamicTile(levelScreen),
		LeverDependentTile(levelScreen),
		SwitchableTile(levelScreen) {
		setInitialState(false);
	}

	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::SwitchableOff; }
};