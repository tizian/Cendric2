#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class InteractComponent;

class CheckpointTile : public LevelDynamicTile {
public:
	CheckpointTile(LevelScreen* levelScreen);
	void init() override;
	void loadAnimation(int skinNr) override; 
	void onRightClick() override;
	void onLeftClick() override;
	void onHit(Spell* spell) override {};
	void onHit(LevelMovableGameObject* mob) override {};
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Checkpoint; }

	void setActive(bool active, bool playSound = true);

private:
	std::string getSpritePath() const override;
	std::string getSoundPath() const override;
	static const float ACTIVATE_RANGE;
	InteractComponent* m_interactComponent;
};