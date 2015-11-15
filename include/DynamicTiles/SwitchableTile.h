#pragma once

#include "global.h"
#include "DynamicTile.h"

class SwitchableTile : public DynamicTile {
public:
	SwitchableTile(Level* level) : DynamicTile(level) {}
	void init() override;
	void setInitialState(bool on);
	void load(int skinNr) override;
	void onHit(Spell* spell) override { /*nop*/ };

	void switchTile();
};