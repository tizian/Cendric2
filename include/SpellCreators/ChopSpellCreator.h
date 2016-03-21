#pragma once

#include <map>

#include "global.h"
#include "Spells/SpellCreator.h"

#include "Spells/ChopSpell.h"

// a class that creates chop spells
class ChopSpellCreator : public SpellCreator {
public:
	ChopSpellCreator(const SpellData &spellData, LevelMovableGameObject *owner);

	void executeSpell(const sf::Vector2f &target) override;
};