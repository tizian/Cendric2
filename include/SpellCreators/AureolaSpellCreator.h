#pragma once

#include <map>

#include "global.h"
#include "SpellCreator.h"

#include "Spells/AureolaSpell.h"

// a class that creates aureola spells
class AureolaSpellCreator : public SpellCreator
{
public:
	AureolaSpellCreator(const SpellBean &spellBean, LevelMovableGameObject *owner);

	void executeSpell(const sf::Vector2f &target) override;

private:
	void addCountModifier(int level) override;
};