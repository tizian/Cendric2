#pragma once

#include "global.h"
#include "Spell.h"

class FearSpell : public Spell
{
public:
	FearSpell(const sf::Time &fearedDuration, int strength);
	void load(const SpellBean& bean, LevelMovableGameObject* mob, sf::Vector2f target, float divergenceAngle) override;
	void execOnHit(LevelMovableGameObject *target) override;

private:
	int m_strength;
	sf::Time m_fearedDuration;
};