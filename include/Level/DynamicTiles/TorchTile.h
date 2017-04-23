#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"

class LightComponent;

class TorchTile final : public LevelDynamicTile {
public:
	TorchTile(LevelScreen* levelScreen);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	int getSkinNr() const;
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Torch; }

private:
	std::string getSpritePath() const override;
	LightComponent* m_lightComponent;
	int m_skinNr;
};