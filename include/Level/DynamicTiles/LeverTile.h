#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Level/DynamicTiles/LeverDependentTile.h"

class LevelMainCharacter;
class InteractComponent;

class LeverTile final : public LevelDynamicTile {
public:
	explicit LeverTile(LevelScreen* levelScreen);
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void onHit(LevelMovableGameObject* mob) override;
	void onRightClick() override;

	void setDependentTiles(const std::vector<LeverDependentTile*>& dependentTiles);
	void switchLever();
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Lever; }

private:
	std::string getSpritePath() const override;
	std::string getSoundPath() const override;

private:
	// the lever can only be used by hand if the main char is in this range.
	// it is however always possible for a telekinesis spell to activate the lever if its projectile reaches the lever.
	static const float ACTIVATE_RANGE;
	std::vector<LeverDependentTile*> m_dependentTiles;
	bool m_isGround = false;
	bool m_isTelekinesisLocked = false;
	InteractComponent* m_interactComponent;
};