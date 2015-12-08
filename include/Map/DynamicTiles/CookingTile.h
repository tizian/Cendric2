#pragma once

#include "global.h"
#include "Map/MapDynamicTile.h"
#include "LightObject.h"

class CookingTile : public MapDynamicTile {
public:
	CookingTile(Map* map);
	void init() override;
	void load(int skinNr) override;

	void setPosition(const sf::Vector2f& pos) override;
	void setDisposed() override;
	void setScreen(Screen* screen) override;

private:
	LightObject* m_lightObject;
};