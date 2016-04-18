#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"
#include "LightObject.h"
#include "GUI/BitmapText.h"

class WaypointTile : public MapDynamicTile {
public:
	WaypointTile(MapScreen* mapScreen);
	void init() override;
	void loadAnimation(int skinNr) override;
	void update(const sf::Time& frameTime) override;
	void onRightClick() override;
	void onLeftClick() override;

	void setActive();
	void setSpawnPosition(int spawnPosition);

private:
	int m_spawnPosition = -1;
	sf::Sound m_sound;
};