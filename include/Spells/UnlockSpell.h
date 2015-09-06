#pragma once

#include "global.h"
#include "Spell.h"

class UnlockSpell : public Spell
{
public:
	UnlockSpell(int strength);
	void load(const SpellBean& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;

private:
	int m_strength;
};