#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class SpikesTopTile final : public LevelDynamicTile {
public:
	SpikesTopTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(LevelMovableGameObject* mob) override;
	void onHit(Spell* spell) override { /*nop*/ };
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::SpikesTop; }

private:
	std::string getSpritePath() const override;
};