#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class InteractComponent;

class CheckpointTile final : public LevelDynamicTile {
public:
	CheckpointTile(LevelScreen* levelScreen);
	bool init(const LevelTileProperties& properties) override;
	void loadAnimation(int skinNr) override; 
	void onRightClick() override;
	void onLeftClick() override;
	void onHit(Spell* spell) override;
	void onHit(LevelMovableGameObject* mob) override {};
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Checkpoint; }

	void setActive(bool active, bool playSound = true);

private:
	std::string getSpritePath() const override;
	std::string getSoundPath() const override;
	void activateMimic();
	static const float ACTIVATE_RANGE;
	InteractComponent* m_interactComponent;

	bool m_isMimic = false;
};