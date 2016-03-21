#pragma once

#include "global.h"
#include "Spells/Spell.h"

class UnlockSpell : public Spell {
public:
	UnlockSpell(int strength);
	void load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) override;

	int getStrength() const;

private:
	int m_strength;
};