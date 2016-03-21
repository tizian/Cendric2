#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class CheckpointTile : public LevelDynamicTile {
public:
	CheckpointTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {};
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override {};
	void onHit(LevelMovableGameObject* mob) override;
};