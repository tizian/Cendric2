#pragma once

#include "global.h"
#include "Spell.h"

class FearSpell : public Spell {
public:
	FearSpell(const sf::Time& fearedDuration, int strength);
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;
	void execOnHit(LevelMovableGameObject* target) override;

private:
	int m_strength;
	sf::Time m_fearedDuration;
};