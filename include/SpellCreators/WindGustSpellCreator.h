#pragma once

#include <map>

#include "global.h"
#include "SpellCreator.h"

#include "Spells/WindGustSpell.h"

// a class that creates windgust spells
class WindGustSpellCreator : public SpellCreator
{
public:
	WindGustSpellCreator(const SpellBean &spellBean, LevelMovableGameObject *owner);

	void executeSpell(const sf::Vector2f &target) override;

private:
	void addRangeModifier(int level) override;
	void addStrengthModifier(int level) override;
};