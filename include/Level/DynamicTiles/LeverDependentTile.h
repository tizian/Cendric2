#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

// Super class for tiles which can be switched by a lever
class LeverDependentTile  {
public:
	virtual ~LeverDependentTile() {}
	virtual void setInitialState(bool on) = 0;
	virtual void switchTile() = 0;
	// returns whether a tile can be switched or if it would possible stuck something
	virtual bool isSwitchable() const = 0;
};