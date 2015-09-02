#pragma once

#include "global.h"
#include "DynamicTile.h"

class CharacterCore;

class CheckpointTile : public DynamicTile
{
public:
	CheckpointTile(CharacterCore* core, Level* level);
	void init() override;
	void load(int skinNr) override;
	void onHit(Spell* spell) override;
	void onHit(LevelMovableGameObject* mob) override;

private:
	CharacterCore* m_core;
};