#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"

class YesOrNoForm;

class TimewarpTile final : public MapDynamicTile {
public:
	TimewarpTile(MapScreen* mapScreen);
	bool init(const MapTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onRightClick() override;
	void onLeftClick() override;

	MapDynamicTileID getDynamicTileID() const override { return MapDynamicTileID::Timewarp; }

private:
	void onInteract();
	void activateTimewarp();
	void onNo();

	std::string getSpritePath() const override;
	std::string getSoundPath() const override;
	YesOrNoForm* m_yesOrNoForm = nullptr;

	static const float ACTIVATE_RANGE;
};