#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class LightComponent;

class TorchTile : public LevelDynamicTile {
public:
	TorchTile(LevelScreen* levelScreen);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;

private:
	std::string getSpritePath() const override;
	LightComponent* m_lightComponent;
};