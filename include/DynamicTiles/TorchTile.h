#pragma once

#include "global.h"
#include "DynamicTile.h"
#include "LightObject.h"

class TorchTile : public DynamicTile {
public:
	TorchTile(Level* level);
	void init() override;
	void load(int skinNr) override;
	void onHit(Spell* spell) override;

	void setPosition(const sf::Vector2f& pos) override;
	void setDisposed() override;
	void setScreen(Screen* screen) override;

private:
	LightObject* m_lightObject;
};