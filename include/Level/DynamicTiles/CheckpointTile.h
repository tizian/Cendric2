#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class CheckpointTile : public LevelDynamicTile {
public:
	CheckpointTile(Level* level) : LevelDynamicTile(level) {};
	void init() override;
	void load(int skinNr) override;
	void onHit(Spell* spell) override {};
	void onHit(LevelMovableGameObject* mob) override;
};