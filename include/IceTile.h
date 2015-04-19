#pragma once

#include "global.h"
#include "DynamicTile.h"

class IceTile : public DynamicTile
{
public:
	void load() override;
	void onHit(Spell* spell) override;

private:
};