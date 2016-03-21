#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Level/DynamicTiles/SwitchableTile.h"

class LevelMainCharacter;

class LeverTile : public LevelDynamicTile {
public:
	LeverTile(LevelScreen* levelScreen);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void onRightClick() override;

	void setDependantTiles(const std::vector<SwitchableTile*>& dependentTiles);
	void switchLever();

private:
	// the lever can only be used by hand if the main char is in this range.
	// it is however always possible for a telekinesis spell to activate the lever if its projectile reaches the lever.
	const float ACTIVATE_RANGE = 100.f;
	std::vector<SwitchableTile*> m_dependentTiles;

	sf::Sound m_sound;
};