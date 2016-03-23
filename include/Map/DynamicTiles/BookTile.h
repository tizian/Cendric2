#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"
#include "GUI/BitmapText.h"
#include "Structs/BookData.h"

// a readable book, starting the book interface when clicked
class BookTile : public MapDynamicTile {
public:
	BookTile(const BookData& data, MapScreen* mapScreen);
	void init() override;
	void loadAnimation(int skinNr) override;
	void onRightClick() override;
	void onLeftClick() override;

private:
	BookData m_data;
};