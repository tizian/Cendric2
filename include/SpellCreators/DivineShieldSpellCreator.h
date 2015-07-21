#pragma once

#include <map>

#include "global.h"
#include "SpellCreator.h"

#include "Spells/DivineShieldSpell.h"

// a class that creates divine shield spells
class DivineShieldSpellCreator : public SpellCreator
{
public:
	DivineShieldSpellCreator(const SpellBean& spellBean, LevelMovableGameObject* owner);

	void executeSpell(sf::Vector2f target) override;

protected:
	void addStrengthModifier(int level);
	int m_additionalResistance;
};