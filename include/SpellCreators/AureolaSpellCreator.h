#pragma once

#include <map>

#include "global.h"
#include "SpellCreator.h"

#include "Spells/AureolaSpell.h"

// a class that creates aureola spells
class AureolaSpellCreator : public SpellCreator
{
public:
	AureolaSpellCreator(const SpellBean& spellBean, LevelMovableGameObject* owner);

	void executeSpell(sf::Vector2f target) override;

	void addCountModifier(int level) override;
};