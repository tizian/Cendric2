#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class SpikesTopTile : public LevelDynamicTile {
public:
	SpikesTopTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {}
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(LevelMovableGameObject* mob) override;
	void onHit(Spell* spell) override { /*nop*/ };
};