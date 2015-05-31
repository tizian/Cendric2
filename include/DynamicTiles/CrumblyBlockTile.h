#pragma once

#include "global.h"
#include "DynamicTile.h"

class CrumblyBlockTile : public DynamicTile
{
public:
	void load() override;
	void onHit(Spell* spell) override;
	void update(const sf::Time& frameTime) override;

private:
	sf::Time m_crumblingTime = sf::seconds(0.4f);
};