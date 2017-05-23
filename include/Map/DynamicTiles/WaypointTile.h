#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"
#include "World/LightObject.h"
#include "GUI/BitmapText.h"

class WaypointTile final : public MapDynamicTile {
public:
	WaypointTile(MapScreen* mapScreen);
	bool init(const MapTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void update(const sf::Time& frameTime) override;
	void onRightClick() override;
	void onLeftClick() override;
	void setPosition(const sf::Vector2f& pos) override;

	void setActive();
	void setSpawnPosition(int spawnPosition);
	MapDynamicTileID getDynamicTileID() const override { return MapDynamicTileID::Waypoint; }

private:
	void activateWaypoint();
	std::string getSpritePath() const override;
	std::string getSoundPath() const override;
	sf::Vector2f m_portPosition;
	int m_spawnPosition = -1;
};