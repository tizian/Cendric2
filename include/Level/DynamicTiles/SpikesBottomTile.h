#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class SpikesBottomTile : public LevelDynamicTile {
public:
	SpikesBottomTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(LevelMovableGameObject* mob) override;
	void onHit(Spell* spell) override { /*nop*/ };

private:
	std::string getSpritePath() const override;
};