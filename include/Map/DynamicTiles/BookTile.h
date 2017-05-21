#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"
#include "GUI/BitmapText.h"

// a readable book, starting the book interface when clicked
class BookTile final : public MapDynamicTile {
public:
	BookTile(MapScreen* mapScreen);
	bool init(const MapTileProperties& properties) override;
	void loadAnimation(int skinNr) override;
	void onRightClick() override;
	void onLeftClick() override;
	MapDynamicTileID getDynamicTileID() const override { return MapDynamicTileID::Book; }

private:
	void startReading();

private:
	std::string getSpritePath() const override;
	static const float RANGE;
	std::string m_bookId;
};