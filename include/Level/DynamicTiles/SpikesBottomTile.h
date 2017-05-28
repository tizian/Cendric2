#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class SpikesBottomTile final : public LevelDynamicTile {
public:
	SpikesBottomTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(LevelMovableGameObject* mob) override;
	void onHit(Spell* spell) override { /*nop*/ };
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::SpikesBottom; }

private:
	std::string getSpritePath() const override;
};