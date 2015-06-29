#pragma once

#include "global.h"
#include "DynamicTile.h"

class IceTile : public DynamicTile
{
public:
	IceTile() : DynamicTile() {}
	void init() override;
	void load(int skinNr) override;
	void onHit(Spell* spell) override;

private:
};