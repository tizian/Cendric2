#pragma once

#include "global.h"
#include "DynamicTile.h"

// tizs awsome water
class SimulatedWaterTile : public DynamicTile
{
public:
	void load(int skinNr) override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;
	void render(sf::RenderTarget& target) override;

private:
};