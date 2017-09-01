#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"
#include "GUI/TooltipWindow.h"
#include "Structs/Condition.h"
#include "World/DoorTile.h"

// a door
class DoorMapTile final : public virtual MapDynamicTile, public virtual DoorTile {
public:
	DoorMapTile(MapScreen* mapScreen);

	void update(const sf::Time& frameTime) override;

	bool init(const MapTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onRightClick() override;

	// resolving diamond of death
	MapDynamicTileID getDynamicTileID() const override { return MapDynamicTileID::Door; }
	GameObjectType getConfiguredType() const override { return MapDynamicTile::getConfiguredType(); }
	void render(sf::RenderTarget& target) override { MapDynamicTile::render(target); }

private:
	float getOpenRange() const override;
	std::string getSpritePath() const override;
	void open() override;
	void close() override;
};