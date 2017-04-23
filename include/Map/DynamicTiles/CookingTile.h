#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"
#include "LightObject.h"
#include "GUI/BitmapText.h"

class CookingTile final : public MapDynamicTile {
public:
	CookingTile(MapScreen* mapScreen);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onRightClick() override;
	MapDynamicTileID getDynamicTileID() const override { return MapDynamicTileID::Cooking; }

private:
	std::string getSpritePath() const override;
	static const float RANGE;
	void startCooking();
};