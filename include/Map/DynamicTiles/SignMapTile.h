#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"
#include "Structs/SignData.h"

// a readable sign
class SignMapTile : public MapDynamicTile {
public:
	SignMapTile(const SignData& data, MapScreen* mapScreen);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onMouseOver() override;

	GameObjectType getConfiguredType() const override;

private:
	SignData m_data;
};