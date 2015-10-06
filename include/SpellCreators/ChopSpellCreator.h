#pragma once

#include <map>

#include "global.h"
#include "SpellCreator.h"

#include "Spells/ChopSpell.h"

// a class that creates chop spells
class ChopSpellCreator : public SpellCreator {
public:
	ChopSpellCreator(const SpellBean &spellBean, LevelMovableGameObject *owner);

	void executeSpell(const sf::Vector2f &target) override;
};