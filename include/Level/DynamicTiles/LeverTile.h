#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Level/DynamicTiles/LeverDependentTile.h"

class LevelMainCharacter;

class LeverTile : public LevelDynamicTile {
public:
	LeverTile(LevelScreen* levelScreen);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void onRightClick() override;

	void setDependentTiles(const std::vector<LeverDependentTile*>& dependentTiles);
	void switchLever();

private:
	std::string getSpritePath() const override;
	std::string getSoundPath() const override;
	// the lever can only be used by hand if the main char is in this range.
	// it is however always possible for a telekinesis spell to activate the lever if its projectile reaches the lever.
	static const float ACTIVATE_RANGE;
	std::vector<LeverDependentTile*> m_dependentTiles;

	sf::Sound m_sound;
};