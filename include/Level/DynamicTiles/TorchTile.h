#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "LightObject.h"

class TorchTile : public LevelDynamicTile {
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