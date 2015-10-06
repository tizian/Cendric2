#pragma once

#include "global.h"
#include "DynamicTile.h"

class CheckpointTile : public DynamicTile {
public:
	CheckpointTile(Level* level) : DynamicTile(level) {};
	void init() override;
	void load(int skinNr) override;
	void onHit(Spell* spell) override;
	void onHit(LevelMovableGameObject* mob) override;
};