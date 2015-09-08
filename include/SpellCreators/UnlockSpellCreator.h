#pragma once

#include "global.h"
#include "SpellCreator.h"

#include "Spells/UnlockSpell.h"

// a class that creates unlock spells
class UnlockSpellCreator : public SpellCreator
{
public:
	UnlockSpellCreator(const SpellBean& spellBean, LevelMovableGameObject* owner);

	void executeSpell(const sf::Vector2f& target) override;

private:
	int m_strength = 1;

	void addStrengthModifier(int level) override;
};