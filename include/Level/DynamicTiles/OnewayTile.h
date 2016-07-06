#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class OnewayTile : public LevelDynamicTile {
public:
	OnewayTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(LevelMovableGameObject* mob) override { /*nop*/ }
	void onHit(Spell* spell) override { /*nop*/ }
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Oneway; }

private:
	std::string getSpritePath() const override;
};